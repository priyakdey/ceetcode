package main

import (
	"flag"
	"fmt"
	"os"
	"path/filepath"
)

const usage = `Usage: generator [options]

Static site generator for ceetcode. Walks the problems source tree, parses
each solution.c frontmatter and notes.md, and renders an HTML page per
problem plus the index page into the output directory.

Defaults are resolved against the repo root (the nearest ancestor of CWD
containing a "problems/" directory), so the generator can be invoked from
any subdirectory of the repo.

Options:
  -dev                include drafts; build, then serve dist/ over HTTP and open in
                      browser. Watches problems/ plus the generator's templates/
                      and assets/ (read from disk, not the embed, so styles and
                      templates hot-reload without recompiling)
  -clean              wipe the output directory before building
  -problems  <path>   problems source directory (default: <repo>/problems)
  -out       <path>   output directory (default: <repo>/dist, or <repo>/dist with -dev)
  -h, -help           show this help and exit

Examples:
  generator                            build to dist, drafts excluded
  generator -dev                       build to dist/, drafts included
  generator -dev -clean                wipe and rebuild dist/
  generator -problems ./problems -out ./public
`

func main() {
	flag.Usage = func() { fmt.Fprint(os.Stderr, usage) }

	var (
		dev      = flag.Bool("dev", false, "")
		clean    = flag.Bool("clean", false, "")
		problems = flag.String("problems", "", "")
		outDir   = flag.String("out", "", "")
	)
	flag.Parse()

	if flag.NArg() > 0 {
		fmt.Fprintf(os.Stderr, "unexpected argument: %q\n\n", flag.Arg(0))
		flag.Usage()
		os.Exit(2)
	}

	root := findRepoRoot()

	probDir := *problems
	if probDir == "" {
		probDir = filepath.Join(root, "problems")
	}

	out := *outDir
	if out == "" {
		name := "dist"
		out = filepath.Join(root, name)
	}

	cfg := Config{
		ProblemsDir:   probDir,
		OutDir:        out,
		IncludeDrafts: *dev,
		Clean:         *clean,
	}

	// In dev, read templates/assets off disk when the source tree is around so
	// they hot-reload like problem content does. Prod always uses the embed.
	watch := []string{probDir}
	if *dev {
		if src := findSrcDir(root); src != "" {
			cfg.SrcDir = src
			watch = append(watch,
				filepath.Join(src, "templates"),
				filepath.Join(src, "assets"),
			)
		}
	}

	if err := Build(cfg); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}

	if *dev {
		rebuild := func() error { return Build(cfg) }
		if err := serve(out, watch, rebuild); err != nil {
			fmt.Fprintln(os.Stderr, "error:", err)
			os.Exit(1)
		}
	}
}

// findSrcDir locates the generator source directory holding templates/ and
// assets/, checking the repo's generator/ folder and CWD. Returns "" when the
// source tree is not available (e.g. the binary was copied elsewhere).
func findSrcDir(root string) string {
	cwd, _ := os.Getwd()
	for _, c := range []string{filepath.Join(root, "generator"), cwd} {
		if c == "" {
			continue
		}
		if info, err := os.Stat(filepath.Join(c, "templates")); err != nil || !info.IsDir() {
			continue
		}
		if info, err := os.Stat(filepath.Join(c, "assets")); err != nil || !info.IsDir() {
			continue
		}
		return c
	}
	return ""
}

// findRepoRoot walks up from CWD looking for a directory containing
// "problems/". Falls back to CWD if none found.
func findRepoRoot() string {
	cwd, err := os.Getwd()
	if err != nil {
		return "."
	}
	for dir := cwd; ; {
		if info, err := os.Stat(filepath.Join(dir, "problems")); err == nil && info.IsDir() {
			return dir
		}
		parent := filepath.Dir(dir)
		if parent == dir {
			return cwd
		}
		dir = parent
	}
}
