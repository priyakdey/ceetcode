package main

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"regexp"
	"strconv"
	"strings"
)

type Frontmatter struct {
	Number     int
	Title      string
	Difficulty string
	Tags       []string
	Link       string
	Draft      bool
}

var fmLineRe = regexp.MustCompile(`^\s*\*\s*@(\w+)\s*:\s*(.*?)\s*$`)

func parseSolution(r io.Reader) (Frontmatter, string, error) {
	br := bufio.NewReader(r)
	first, err := br.ReadString('\n')
	if err != nil && err != io.EOF {
		return Frontmatter{}, "", err
	}
	if !strings.HasPrefix(strings.TrimSpace(first), "/**") {
		return Frontmatter{}, "", errors.New("solution.c must start with /**")
	}

	var fm Frontmatter
	for {
		line, err := br.ReadString('\n')
		if strings.HasPrefix(strings.TrimSpace(line), "*/") {
			break
		}
		if m := fmLineRe.FindStringSubmatch(line); m != nil {
			if err := assignField(&fm, m[1], m[2]); err != nil {
				return fm, "", err
			}
		}
		if err == io.EOF {
			return fm, "", errors.New("unterminated frontmatter")
		}
		if err != nil {
			return fm, "", err
		}
	}

	rest, err := io.ReadAll(br)
	if err != nil {
		return fm, "", err
	}
	code := strings.TrimLeft(string(rest), "\n")
	return fm, code, nil
}

func assignField(fm *Frontmatter, key, val string) error {
	switch key {
	case "number":
		n, err := strconv.Atoi(val)
		if err != nil {
			return fmt.Errorf("invalid @number %q: %w", val, err)
		}
		fm.Number = n
	case "title":
		fm.Title = val
	case "difficulty":
		fm.Difficulty = strings.ToLower(val)
	case "tags":
		for _, p := range strings.Split(val, ",") {
			t := strings.TrimSpace(p)
			if t != "" {
				fm.Tags = append(fm.Tags, strings.ToLower(t))
			}
		}
	case "link":
		fm.Link = val
	case "draft":
		fm.Draft = strings.EqualFold(strings.TrimSpace(val), "true")
	}
	return nil
}

func slugFromLink(link, fallbackTitle string) string {
	if i := strings.Index(link, "/problems/"); i >= 0 {
		rest := link[i+len("/problems/"):]
		if j := strings.Index(rest, "/"); j >= 0 {
			return rest[:j]
		}
		return rest
	}
	var b strings.Builder
	prevHyphen := true
	for _, r := range strings.ToLower(fallbackTitle) {
		if (r >= 'a' && r <= 'z') || (r >= '0' && r <= '9') {
			b.WriteRune(r)
			prevHyphen = false
		} else if !prevHyphen {
			b.WriteByte('-')
			prevHyphen = true
		}
	}
	return strings.Trim(b.String(), "-")
}
