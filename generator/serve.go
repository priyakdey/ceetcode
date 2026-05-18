package main

import (
	"fmt"
	"net"
	"net/http"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"sync"
	"time"
)

const (
	defaultPort  = 6969
	pollInterval = 500 * time.Millisecond
)

const devReloadScript = `<script>new EventSource('/_dev/reload').onmessage=()=>location.reload();</script>`

func serve(dir string, watchRoots []string, rebuild func() error) error {
	ln, err := net.Listen("tcp", fmt.Sprintf("127.0.0.1:%d", defaultPort))
	if err != nil {
		ln, err = net.Listen("tcp", "127.0.0.1:0")
		if err != nil {
			return fmt.Errorf("listen: %w", err)
		}
	}

	port := ln.Addr().(*net.TCPAddr).Port
	url := fmt.Sprintf("http://localhost:%d/", port)
	fmt.Printf("serving %s at %s (Ctrl+C to stop)\n", dir, url)

	hub := newReloadHub()
	if rebuild != nil && len(watchRoots) > 0 {
		go watchLoop(watchRoots, func() {
			if err := rebuild(); err != nil {
				fmt.Fprintf(os.Stderr, "rebuild error: %v\n", err)
				return
			}
			fmt.Println("rebuilt, reloading clients")
			hub.broadcast()
		})
	}

	mux := http.NewServeMux()
	mux.HandleFunc("/_dev/reload", hub.handle)
	mux.Handle("/", injectReload(http.FileServer(http.Dir(dir))))

	if err := openBrowser(url); err != nil {
		fmt.Fprintf(os.Stderr, "warn: could not open browser: %v\n", err)
	}

	return http.Serve(ln, mux)
}

// ── reload hub: fans out a reload event to every connected SSE client ──

type reloadHub struct {
	mu   sync.Mutex
	subs map[chan struct{}]struct{}
}

func newReloadHub() *reloadHub {
	return &reloadHub{subs: map[chan struct{}]struct{}{}}
}

func (h *reloadHub) handle(w http.ResponseWriter, r *http.Request) {
	flusher, ok := w.(http.Flusher)
	if !ok {
		http.Error(w, "streaming unsupported", http.StatusInternalServerError)
		return
	}
	w.Header().Set("Content-Type", "text/event-stream")
	w.Header().Set("Cache-Control", "no-cache")
	w.Header().Set("Connection", "keep-alive")

	ch := make(chan struct{}, 1)
	h.mu.Lock()
	h.subs[ch] = struct{}{}
	h.mu.Unlock()
	defer func() {
		h.mu.Lock()
		delete(h.subs, ch)
		h.mu.Unlock()
	}()

	fmt.Fprint(w, ": connected\n\n")
	flusher.Flush()

	for {
		select {
		case <-ch:
			fmt.Fprint(w, "data: reload\n\n")
			flusher.Flush()
		case <-r.Context().Done():
			return
		}
	}
}

func (h *reloadHub) broadcast() {
	h.mu.Lock()
	defer h.mu.Unlock()
	for ch := range h.subs {
		select {
		case ch <- struct{}{}:
		default:
		}
	}
}

// ── HTML injection: append the reload script before </body> ──

func injectReload(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		iw := &injectingWriter{ResponseWriter: w}
		next.ServeHTTP(iw, r)
		iw.commit()
	})
}

type injectingWriter struct {
	http.ResponseWriter
	buf         []byte
	status      int
	wroteHeader bool
	isHTML      bool
}

func (iw *injectingWriter) WriteHeader(code int) {
	if iw.wroteHeader {
		return
	}
	iw.status = code
	iw.wroteHeader = true
	iw.isHTML = strings.Contains(iw.Header().Get("Content-Type"), "text/html")
	if !iw.isHTML {
		iw.ResponseWriter.WriteHeader(code)
	}
}

func (iw *injectingWriter) Write(b []byte) (int, error) {
	if !iw.wroteHeader {
		iw.WriteHeader(http.StatusOK)
	}
	if !iw.isHTML {
		return iw.ResponseWriter.Write(b)
	}
	iw.buf = append(iw.buf, b...)
	return len(b), nil
}

func (iw *injectingWriter) commit() {
	if !iw.isHTML {
		return
	}
	body := string(iw.buf)
	if idx := strings.LastIndex(body, "</body>"); idx >= 0 {
		body = body[:idx] + devReloadScript + body[idx:]
	} else {
		body += devReloadScript
	}
	iw.Header().Del("Content-Length")
	iw.ResponseWriter.WriteHeader(iw.status)
	iw.ResponseWriter.Write([]byte(body))
}

// ── filesystem polling: detect changes by walking + comparing mtimes ──

func watchLoop(roots []string, onChange func()) {
	prev := snapshot(roots)
	for {
		time.Sleep(pollInterval)
		cur := snapshot(roots)
		if !sameSnapshot(prev, cur) {
			onChange()
			prev = snapshot(roots)
		}
	}
}

func snapshot(roots []string) map[string]time.Time {
	out := map[string]time.Time{}
	for _, root := range roots {
		filepath.Walk(root, func(p string, info os.FileInfo, err error) error {
			if err != nil || info.IsDir() {
				return nil
			}
			out[p] = info.ModTime()
			return nil
		})
	}
	return out
}

func sameSnapshot(a, b map[string]time.Time) bool {
	if len(a) != len(b) {
		return false
	}
	for k, v := range a {
		bv, ok := b[k]
		if !ok || !bv.Equal(v) {
			return false
		}
	}
	return true
}

func openBrowser(url string) error {
	if b := os.Getenv("BROWSER"); b != "" {
		return exec.Command(b, url).Start()
	}
	var cmd *exec.Cmd
	switch runtime.GOOS {
	case "linux":
		cmd = exec.Command("xdg-open", url)
	case "darwin":
		cmd = exec.Command("open", url)
	case "windows":
		cmd = exec.Command("rundll32", "url.dll,FileProtocolHandler", url)
	default:
		return fmt.Errorf("unsupported platform: %s", runtime.GOOS)
	}
	return cmd.Start()
}
