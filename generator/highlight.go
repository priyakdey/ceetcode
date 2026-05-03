package main

import (
	"fmt"
	"html"
	"strings"
)

type tokKind int

const (
	tNone tokKind = iota
	tPP
	tKw
	tTy
	tFn
	tStr
	tNum
	tCmt
)

type tok struct {
	kind tokKind
	text string
}

func (k tokKind) class() string {
	switch k {
	case tPP:
		return "pp"
	case tKw:
		return "kw"
	case tTy:
		return "ty"
	case tFn:
		return "fn"
	case tStr:
		return "str"
	case tNum:
		return "num"
	case tCmt:
		return "cmt"
	}
	return ""
}

var cKeywords = map[string]bool{
	"auto": true, "break": true, "case": true, "const": true, "continue": true,
	"default": true, "do": true, "else": true, "enum": true, "extern": true,
	"for": true, "goto": true, "if": true, "inline": true, "register": true,
	"restrict": true, "return": true, "sizeof": true, "static": true,
	"struct": true, "switch": true, "typedef": true, "union": true,
	"volatile": true, "while": true,
	"_Alignas": true, "_Alignof": true, "_Atomic": true, "_Bool": true,
	"_Generic": true, "_Noreturn": true, "_Static_assert": true, "_Thread_local": true,
}

var cTypes = map[string]bool{
	"char": true, "short": true, "int": true, "long": true,
	"float": true, "double": true, "void": true,
	"signed": true, "unsigned": true, "bool": true,
	"size_t": true, "ssize_t": true, "ptrdiff_t": true,
	"intptr_t": true, "uintptr_t": true,
	"int8_t": true, "int16_t": true, "int32_t": true, "int64_t": true,
	"uint8_t": true, "uint16_t": true, "uint32_t": true, "uint64_t": true,
	"FILE": true,
}

func highlightC(code string) string {
	if code == "" {
		return ""
	}
	return wrapLines(tokenize(code))
}

func tokenize(src string) []tok {
	r := []rune(src)
	var toks []tok
	atLineStart := true
	for i := 0; i < len(r); {
		c := r[i]
		if c == '\n' {
			toks = append(toks, tok{tNone, "\n"})
			i++
			atLineStart = true
			continue
		}
		if c == ' ' || c == '\t' {
			j := i
			for j < len(r) && (r[j] == ' ' || r[j] == '\t') {
				j++
			}
			toks = append(toks, tok{tNone, string(r[i:j])})
			i = j
			continue
		}
		if atLineStart && c == '#' {
			j := i + 1
			for j < len(r) && (isAlpha(r[j]) || r[j] == '_') {
				j++
			}
			toks = append(toks, tok{tPP, string(r[i:j])})
			i = j
			atLineStart = false
			continue
		}
		atLineStart = false

		if c == '/' && i+1 < len(r) && r[i+1] == '*' {
			j := i + 2
			for j+1 < len(r) && !(r[j] == '*' && r[j+1] == '/') {
				j++
			}
			end := j + 2
			if end > len(r) {
				end = len(r)
			}
			toks = append(toks, tok{tCmt, string(r[i:end])})
			i = end
			continue
		}
		if c == '/' && i+1 < len(r) && r[i+1] == '/' {
			j := i
			for j < len(r) && r[j] != '\n' {
				j++
			}
			toks = append(toks, tok{tCmt, string(r[i:j])})
			i = j
			continue
		}
		if c == '"' || c == '\'' {
			end := scanQuoted(r, i, c)
			toks = append(toks, tok{tStr, string(r[i:end])})
			i = end
			continue
		}
		if c == '<' && lastSignificantWas(toks, "#include") {
			j := i + 1
			for j < len(r) && r[j] != '>' && r[j] != '\n' {
				j++
			}
			end := j
			if end < len(r) && r[end] == '>' {
				end++
			}
			toks = append(toks, tok{tStr, string(r[i:end])})
			i = end
			continue
		}
		if isDigit(c) || (c == '.' && i+1 < len(r) && isDigit(r[i+1])) {
			j := i
			for j < len(r) && isNumPart(r[j]) {
				j++
			}
			toks = append(toks, tok{tNum, string(r[i:j])})
			i = j
			continue
		}
		if isAlpha(c) || c == '_' {
			j := i
			for j < len(r) && (isAlnum(r[j]) || r[j] == '_') {
				j++
			}
			ident := string(r[i:j])
			toks = append(toks, tok{classifyIdent(ident, r, j), ident})
			i = j
			continue
		}
		toks = append(toks, tok{tNone, string(c)})
		i++
	}
	return toks
}

func scanQuoted(r []rune, start int, quote rune) int {
	j := start + 1
	for j < len(r) {
		if r[j] == '\\' && j+1 < len(r) {
			j += 2
			continue
		}
		if r[j] == quote || r[j] == '\n' {
			break
		}
		j++
	}
	if j < len(r) && r[j] == quote {
		j++
	}
	return j
}

func classifyIdent(ident string, r []rune, j int) tokKind {
	switch {
	case cKeywords[ident]:
		return tKw
	case cTypes[ident]:
		return tTy
	case isUserType(ident):
		return tTy
	}
	k := j
	for k < len(r) && (r[k] == ' ' || r[k] == '\t') {
		k++
	}
	if k < len(r) && r[k] == '(' {
		return tFn
	}
	return tNone
}

// User type heuristic: starts uppercase, has at least one lowercase letter
// (excludes ALL_CAPS macros like NULL, EMPTY).
func isUserType(ident string) bool {
	if len(ident) == 0 {
		return false
	}
	if ident[0] < 'A' || ident[0] > 'Z' {
		return false
	}
	for _, c := range ident {
		if c >= 'a' && c <= 'z' {
			return true
		}
	}
	return false
}

func lastSignificantWas(toks []tok, want string) bool {
	for i := len(toks) - 1; i >= 0; i-- {
		t := toks[i]
		if t.kind == tNone && strings.TrimSpace(t.text) == "" {
			continue
		}
		return t.text == want
	}
	return false
}

func isAlpha(r rune) bool { return (r >= 'a' && r <= 'z') || (r >= 'A' && r <= 'Z') }
func isDigit(r rune) bool { return r >= '0' && r <= '9' }
func isAlnum(r rune) bool { return isAlpha(r) || isDigit(r) }

func isNumPart(r rune) bool {
	return isDigit(r) || r == '.' ||
		r == 'x' || r == 'X' || r == 'u' || r == 'U' ||
		r == 'l' || r == 'L' || r == 'f' || r == 'F' ||
		(r >= 'a' && r <= 'f') || (r >= 'A' && r <= 'F')
}

func wrapLines(toks []tok) string {
	var lines [][]tok
	var cur []tok
	flush := func() {
		lines = append(lines, cur)
		cur = nil
	}
	for _, t := range toks {
		if t.kind == tNone && t.text == "\n" {
			flush()
			continue
		}
		if strings.Contains(t.text, "\n") {
			parts := strings.Split(t.text, "\n")
			for k, p := range parts {
				if p != "" {
					cur = append(cur, tok{t.kind, p})
				}
				if k < len(parts)-1 {
					flush()
				}
			}
			continue
		}
		cur = append(cur, t)
	}
	if cur != nil {
		flush()
	}
	for len(lines) > 0 && len(lines[len(lines)-1]) == 0 {
		lines = lines[:len(lines)-1]
	}

	width := len(fmt.Sprintf("%d", len(lines)))
	if width < 2 {
		width = 2
	}
	var b strings.Builder
	for n, ln := range lines {
		fmt.Fprintf(&b, `<div class="code-line"><span class="line-num">%*d</span><span class="line-content">`, width, n+1)
		for _, t := range ln {
			esc := html.EscapeString(t.text)
			if cls := t.kind.class(); cls != "" {
				fmt.Fprintf(&b, `<span class="%s">%s</span>`, cls, esc)
			} else {
				b.WriteString(esc)
			}
		}
		b.WriteString("</span></div>")
	}
	return b.String()
}
