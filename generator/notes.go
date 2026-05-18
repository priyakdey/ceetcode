package main

import (
	"html"
	"io"
	"regexp"
	"strings"
)

type Notes struct {
	Approach        string
	Summary         string
	TimeComplexity  string
	SpaceComplexity string
	TimeNote        string
	SpaceNote       string
}

var (
	codeRe   = regexp.MustCompile("`([^`]+)`")
	boldRe   = regexp.MustCompile(`\*\*([^*]+)\*\*`)
	bulletRe = regexp.MustCompile(`(?i)^-\s*(Time|Space)\s*:\s*(.*)$`)
)

func parseNotes(r io.Reader) (Notes, error) {
	data, err := io.ReadAll(r)
	if err != nil {
		return Notes{}, err
	}
	return parseNotesText(string(data)), nil
}

func parseNotesText(text string) Notes {
	var n Notes
	sections := splitSections(text)
	if app, ok := sections["approach"]; ok {
		n.Approach = renderApproach(app)
		n.Summary = summarize(app, 155)
	}
	if comp, ok := sections["complexity"]; ok {
		n.TimeComplexity, n.TimeNote, n.SpaceComplexity, n.SpaceNote = parseComplexity(comp)
	}
	return n
}

func splitSections(text string) map[string]string {
	out := map[string]string{}
	var current string
	var buf strings.Builder
	for _, line := range strings.Split(text, "\n") {
		if strings.HasPrefix(line, "## ") {
			if current != "" {
				out[current] = buf.String()
			}
			current = strings.ToLower(strings.TrimSpace(line[3:]))
			buf.Reset()
			continue
		}
		buf.WriteString(line)
		buf.WriteByte('\n')
	}
	if current != "" {
		out[current] = buf.String()
	}
	return out
}

func renderApproach(text string) string {
	var paragraphs []string
	for _, p := range strings.Split(strings.TrimSpace(text), "\n\n") {
		p = strings.TrimSpace(p)
		if p == "" {
			continue
		}
		p = strings.Join(strings.Fields(p), " ")
		paragraphs = append(paragraphs, "<p>"+renderInline(p)+"</p>")
	}
	return strings.Join(paragraphs, "\n")
}

// summarize returns the first paragraph of approach text as plain prose,
// stripped of markdown markers and clamped to roughly maxLen characters
// (truncated at a word boundary, with an ellipsis appended if shortened).
// Suitable for <meta name="description"> and og:description.
func summarize(text string, maxLen int) string {
	first := strings.TrimSpace(strings.SplitN(strings.TrimSpace(text), "\n\n", 2)[0])
	if first == "" {
		return ""
	}
	first = strings.Join(strings.Fields(first), " ")
	first = strings.ReplaceAll(first, "`", "")
	first = boldRe.ReplaceAllString(first, "$1")
	if len(first) <= maxLen {
		return first
	}
	cut := first[:maxLen]
	if i := strings.LastIndex(cut, " "); i > 0 {
		cut = cut[:i]
	}
	return strings.TrimRight(cut, " ,.;:-") + "…"
}

func renderInline(s string) string {
	s = html.EscapeString(s)
	s = codeRe.ReplaceAllString(s, "<code>$1</code>")
	s = boldRe.ReplaceAllString(s, "<strong>$1</strong>")
	return s
}

func parseComplexity(text string) (timeC, timeNote, spaceC, spaceNote string) {
	type bullet struct{ kind, body string }
	var bullets []bullet
	curIdx := -1
	for _, line := range strings.Split(text, "\n") {
		trimmed := strings.TrimSpace(line)
		if trimmed == "" {
			curIdx = -1
			continue
		}
		if m := bulletRe.FindStringSubmatch(trimmed); m != nil {
			bullets = append(bullets, bullet{
				kind: strings.ToLower(m[1]),
				body: strings.TrimSpace(m[2]),
			})
			curIdx = len(bullets) - 1
			continue
		}
		if curIdx >= 0 {
			if bullets[curIdx].body != "" {
				bullets[curIdx].body += " "
			}
			bullets[curIdx].body += trimmed
		}
	}

	for _, b := range bullets {
		expr, note := splitExpressionNote(b.body)
		expr = strings.ReplaceAll(expr, "`", "")
		var rendered string
		if note != "" {
			rendered = renderInline(note)
		}
		switch b.kind {
		case "time":
			timeC, timeNote = expr, rendered
		case "space":
			spaceC, spaceNote = expr, rendered
		}
	}
	return
}

// splitExpressionNote splits a complexity bullet body into the expression
// (e.g. "O(n)") and an optional rationale, divided by the first " - " or
// " — " that sits outside backtick-delimited code spans.
func splitExpressionNote(s string) (expr, note string) {
	inCode := false
	for i := 0; i < len(s); i++ {
		c := s[i]
		if c == '`' {
			inCode = !inCode
			continue
		}
		if inCode || c != ' ' {
			continue
		}
		rest := s[i:]
		if strings.HasPrefix(rest, " - ") {
			return strings.TrimSpace(s[:i]), strings.TrimSpace(s[i+3:])
		}
		if strings.HasPrefix(rest, " — ") {
			return strings.TrimSpace(s[:i]), strings.TrimSpace(s[i+len(" — "):])
		}
	}
	return strings.TrimSpace(s), ""
}
