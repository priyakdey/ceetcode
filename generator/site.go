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

const sourceURLBase = "https://github.com/priyakdey/ceetcode/blob/main/problems/"

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
	}, nil
}

func linkNav(probs []Problem) {
	year := time.Now().Year()
	for i := range probs {
		probs[i].Year = year
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

func titleCase(s string) string {
	if s == "" {
		return ""
	}
	return strings.ToUpper(s[:1]) + s[1:]
}
