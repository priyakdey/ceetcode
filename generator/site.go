package main

import (
	"embed"
	"encoding/json"
	"fmt"
	"html/template"
	"io/fs"
	"os"
	"path/filepath"
	"sort"
	"strings"
	"time"
)

//go:embed templates
var templatesFS embed.FS

//go:embed assets
var assetsFS embed.FS

const (
	sourceURLBase = "https://github.com/priyakdey/ceetcode/blob/main/problems/"
	siteURL       = "https://ceetcode.com"
	siteName      = "ceetcode"
	authorName    = "Priyak Dey"
	authorURL     = "https://priyakdey.com"
	ogImagePath   = "/favico/android-chrome-512x512.png"
)

type Config struct {
	ProblemsDir   string
	OutDir        string
	IncludeDrafts bool
	Clean         bool
}

type NavRef struct {
	Number int
	Title  string
	Slug   string
}

type Problem struct {
	Number          int
	Title           string
	Slug            string
	Difficulty      string
	DifficultyTitle string
	Tags            []string
	LeetcodeURL     string
	SourceURL       string
	Code            template.HTML
	Approach        template.HTML
	TimeComplexity  string
	SpaceComplexity string
	TimeNote        template.HTML
	SpaceNote       template.HTML
	Draft           bool
	FolderName      string
	Prev, Next      *NavRef
	Year            int

	Description  string
	CanonicalURL string
	OGImageURL   string
	JSONLD       template.JS
	SiteName     string
	AuthorName   string
	AuthorURL    string
}

type indexEntry struct {
	Number     int      `json:"number"`
	Title      string   `json:"title"`
	Slug       string   `json:"slug"`
	Difficulty string   `json:"difficulty"`
	Tags       []string `json:"tags"`
}

func Build(cfg Config) error {
	probs, err := loadAll(cfg.ProblemsDir, cfg.IncludeDrafts)
	if err != nil {
		return err
	}
	sort.Slice(probs, func(i, j int) bool { return probs[i].Number < probs[j].Number })
	linkNav(probs)

	if cfg.Clean {
		if err := os.RemoveAll(cfg.OutDir); err != nil {
			return fmt.Errorf("clean %s: %w", cfg.OutDir, err)
		}
	}
	if err := os.MkdirAll(cfg.OutDir, 0o755); err != nil {
		return err
	}
	if err := copyAssets(cfg.OutDir); err != nil {
		return fmt.Errorf("copy assets: %w", err)
	}
	if err := renderProblems(probs, cfg.OutDir); err != nil {
		return err
	}
	if err := writeIndexJSON(probs, cfg.OutDir); err != nil {
		return err
	}
	if err := writeSitemap(probs, cfg.OutDir); err != nil {
		return err
	}
	if err := writeRobots(cfg.OutDir); err != nil {
		return err
	}

	mode := "prod"
	if cfg.IncludeDrafts {
		mode = "dev"
	}
	fmt.Printf("built %d problems (%s) -> %s/\n", len(probs), mode, cfg.OutDir)
	return nil
}

func loadAll(dir string, includeDrafts bool) ([]Problem, error) {
	entries, err := os.ReadDir(dir)
	if err != nil {
		return nil, fmt.Errorf("read %s: %w", dir, err)
	}
	var probs []Problem
	for _, e := range entries {
		if !e.IsDir() {
			continue
		}
		p, err := loadProblem(filepath.Join(dir, e.Name()), e.Name())
		if err != nil {
			return nil, fmt.Errorf("%s: %w", e.Name(), err)
		}
		if p.Draft && !includeDrafts {
			continue
		}
		probs = append(probs, p)
	}
	return probs, nil
}

func loadProblem(dir, folder string) (Problem, error) {
	cf, err := os.Open(filepath.Join(dir, "solution.c"))
	if err != nil {
		return Problem{}, err
	}
	defer cf.Close()
	fm, code, err := parseSolution(cf)
	if err != nil {
		return Problem{}, err
	}

	var notes Notes
	if nf, err := os.Open(filepath.Join(dir, "notes.md")); err == nil {
		notes, _ = parseNotes(nf)
		nf.Close()
	}

	slug := slugFromLink(fm.Link, fm.Title)
	if slug == "" {
		if parts := strings.SplitN(folder, "-", 2); len(parts) == 2 {
			slug = parts[1]
		} else {
			slug = folder
		}
	}

	return Problem{
		Number:          fm.Number,
		Title:           fm.Title,
		Slug:            slug,
		Difficulty:      fm.Difficulty,
		DifficultyTitle: titleCase(fm.Difficulty),
		Tags:            fm.Tags,
		LeetcodeURL:     fm.Link,
		SourceURL:       sourceURLBase + folder + "/solution.c",
		Code:            template.HTML(highlightC(code)),
		Approach:        template.HTML(notes.Approach),
		TimeComplexity:  notes.TimeComplexity,
		SpaceComplexity: notes.SpaceComplexity,
		TimeNote:        template.HTML(notes.TimeNote),
		SpaceNote:       template.HTML(notes.SpaceNote),
		Draft:           fm.Draft,
		FolderName:      folder,
		Description:     buildDescription(fm.Title, titleCase(fm.Difficulty), notes.Summary),
	}, nil
}

func linkNav(probs []Problem) {
	year := time.Now().Year()
	for i := range probs {
		probs[i].Year = year
		probs[i].CanonicalURL = siteURL + "/" + probs[i].Slug + ".html"
		probs[i].OGImageURL = siteURL + ogImagePath
		probs[i].SiteName = siteName
		probs[i].AuthorName = authorName
		probs[i].AuthorURL = authorURL
		probs[i].JSONLD = template.JS(buildJSONLD(&probs[i]))
		if i > 0 {
			probs[i].Prev = &NavRef{
				Number: probs[i-1].Number, Title: probs[i-1].Title, Slug: probs[i-1].Slug,
			}
		}
		if i < len(probs)-1 {
			probs[i].Next = &NavRef{
				Number: probs[i+1].Number, Title: probs[i+1].Title, Slug: probs[i+1].Slug,
			}
		}
	}
}

func renderProblems(probs []Problem, outDir string) error {
	tmpl, err := template.ParseFS(templatesFS, "templates/problem.html.tmpl")
	if err != nil {
		return fmt.Errorf("parse template: %w", err)
	}
	for _, p := range probs {
		path := filepath.Join(outDir, p.Slug+".html")
		f, err := os.Create(path)
		if err != nil {
			return err
		}
		if err := tmpl.Execute(f, p); err != nil {
			f.Close()
			return fmt.Errorf("render %s: %w", p.Slug, err)
		}
		f.Close()
	}
	return nil
}

func writeIndexJSON(probs []Problem, outDir string) error {
	entries := make([]indexEntry, len(probs))
	for i, p := range probs {
		entries[i] = indexEntry{
			Number: p.Number, Title: p.Title, Slug: p.Slug,
			Difficulty: p.Difficulty, Tags: p.Tags,
		}
	}
	f, err := os.Create(filepath.Join(outDir, "problems.json"))
	if err != nil {
		return err
	}
	defer f.Close()
	enc := json.NewEncoder(f)
	enc.SetIndent("", "  ")
	return enc.Encode(map[string]any{"PROBLEMS": entries})
}

func copyAssets(outDir string) error {
	return fs.WalkDir(assetsFS, "assets", func(p string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}
		rel, err := filepath.Rel("assets", p)
		if err != nil {
			return err
		}
		dst := filepath.Join(outDir, rel)
		if d.IsDir() {
			return os.MkdirAll(dst, 0o755)
		}
		data, err := assetsFS.ReadFile(p)
		if err != nil {
			return err
		}
		return os.WriteFile(dst, data, 0o644)
	})
}

func buildDescription(title, difficulty, summary string) string {
	if summary != "" {
		return summary
	}
	if difficulty != "" {
		return title + " (" + difficulty + ") — LeetCode problem solved in C with annotated source and complexity analysis."
	}
	return title + " — LeetCode problem solved in C with annotated source and complexity analysis."
}

func buildJSONLD(p *Problem) string {
	keywords := append([]string{p.Title, "leetcode", "c programming"}, p.Tags...)
	doc := map[string]any{
		"@context":            "https://schema.org",
		"@type":               "TechArticle",
		"headline":            p.Title,
		"name":                p.Title,
		"description":         p.Description,
		"url":                 p.CanonicalURL,
		"inLanguage":          "en",
		"programmingLanguage": "C",
		"keywords":            strings.Join(keywords, ", "),
		"image":               p.OGImageURL,
		"mainEntityOfPage": map[string]any{
			"@type": "WebPage",
			"@id":   p.CanonicalURL,
		},
		"author": map[string]any{
			"@type": "Person",
			"name":  authorName,
			"url":   authorURL,
		},
		"publisher": map[string]any{
			"@type": "Person",
			"name":  authorName,
			"url":   authorURL,
		},
		"isPartOf": map[string]any{
			"@type": "WebSite",
			"name":  siteName,
			"url":   siteURL,
		},
	}
	b, err := json.Marshal(doc)
	if err != nil {
		return "{}"
	}
	return string(b)
}

func writeSitemap(probs []Problem, outDir string) error {
	var b strings.Builder
	b.WriteString(`<?xml version="1.0" encoding="UTF-8"?>` + "\n")
	b.WriteString(`<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">` + "\n")
	today := time.Now().UTC().Format("2006-01-02")
	b.WriteString("  <url>\n")
	b.WriteString("    <loc>" + siteURL + "/</loc>\n")
	b.WriteString("    <lastmod>" + today + "</lastmod>\n")
	b.WriteString("    <changefreq>weekly</changefreq>\n")
	b.WriteString("    <priority>1.0</priority>\n")
	b.WriteString("  </url>\n")
	for _, p := range probs {
		b.WriteString("  <url>\n")
		b.WriteString("    <loc>" + siteURL + "/" + p.Slug + ".html</loc>\n")
		b.WriteString("    <lastmod>" + today + "</lastmod>\n")
		b.WriteString("    <changefreq>monthly</changefreq>\n")
		b.WriteString("    <priority>0.8</priority>\n")
		b.WriteString("  </url>\n")
	}
	b.WriteString("</urlset>\n")
	return os.WriteFile(filepath.Join(outDir, "sitemap.xml"), []byte(b.String()), 0o644)
}

func writeRobots(outDir string) error {
	body := "User-agent: *\nAllow: /\n\nSitemap: " + siteURL + "/sitemap.xml\n"
	return os.WriteFile(filepath.Join(outDir, "robots.txt"), []byte(body), 0o644)
}

func titleCase(s string) string {
	if s == "" {
		return ""
	}
	return strings.ToUpper(s[:1]) + s[1:]
}
