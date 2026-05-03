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
problem plus a problems.json index into the output directory.

Defaults are resolved against the repo root (the nearest ancestor of CWD
containing a "problems/" directory), so the generator can be invoked from
any subdirectory of the repo.

Options:
  -dev                include drafts; output to dist/ unless -out is set
  -clean              wipe the output directory before building
  -problems  <path>   problems source directory (default: <repo>/problems)
  -out       <path>   output directory (default: <repo>/docs, or <repo>/dist with -dev)
  -h, -help           show this help and exit

Examples:
  generator                            build to docs/, drafts excluded
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
		name := "docs"
		if *dev {
			name = "dist"
		}
		out = filepath.Join(root, name)
	}

	cfg := Config{
		ProblemsDir:   probDir,
		OutDir:        out,
		IncludeDrafts: *dev,
		Clean:         *clean,
	}
	if err := Build(cfg); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
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
