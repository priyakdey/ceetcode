# ceetcode

leetcode, solved in c — because why not.

A growing collection of LeetCode problems written in plain C. No external libraries, no hand-holding — just pointers, manual memory, and the occasional segfault. Built for fun, learning, and a healthy disregard for convenience.

Each solution comes with annotated source, complexity analysis, and the thought process behind the approach, published at **[ceetcode.com](https://ceetcode.com)**.

## why C?

Because the constraints sharpen the thinking. There's no `HashMap<K,V>`, no `Vec<T>`, no `.sort()`. You build what you need, manage every byte, and earn every segfault. It's recreational programming — the goal is to enjoy the puzzle, not to climb the LeetCode leaderboard.

## what's in here

- `problems/NNNN-slug/` — one folder per problem (`solution.c` with frontmatter + `notes.md` with approach & complexity)
- `generator/` — a small Go static site generator that turns `problems/` into the published site. Stdlib only, in keeping with the spirit. Hand-rolled C tokenizer for syntax highlighting.
- `.github/workflows/deploy.yml` — builds and ships to GitHub Pages on every push to `main`

## building locally

```sh
cd generator && go build -o generator .

./generator           # prod build → docs/
./generator -dev      # includes drafts → dist/
./generator -h        # usage
```

## license

MIT — see [LICENSE](LICENSE).
