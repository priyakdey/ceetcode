package main

import (
	"html"
	"io"
	"regexp"
	"strings"
)

type Notes struct {
	Approach        string
	TimeComplexity  string
	SpaceComplexity string
}

var (
	codeRe    = regexp.MustCompile("`([^`]+)`")
	boldRe    = regexp.MustCompile(`\*\*([^*]+)\*\*`)
	complexRe = regexp.MustCompile(`(?i)^-\s*(Time|Space)\s*:\s*(.+)$`)
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
	}
	if comp, ok := sections["complexity"]; ok {
		n.TimeComplexity, n.SpaceComplexity = parseComplexity(comp)
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
		p = html.EscapeString(p)
		p = codeRe.ReplaceAllString(p, "<code>$1</code>")
		p = boldRe.ReplaceAllString(p, "<strong>$1</strong>")
		paragraphs = append(paragraphs, "<p>"+p+"</p>")
	}
	return strings.Join(paragraphs, "\n")
}

func parseComplexity(text string) (timeC, spaceC string) {
	for _, line := range strings.Split(text, "\n") {
		m := complexRe.FindStringSubmatch(strings.TrimSpace(line))
		if m == nil {
			continue
		}
		val := strings.TrimSpace(m[2])
		switch strings.ToLower(m[1]) {
		case "time":
			timeC = val
		case "space":
			spaceC = val
		}
	}
	return
}
