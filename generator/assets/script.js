// ─── Theme ───────────────────────────────────────────────────────
// The initial value is resolved by an inline script in <head> to avoid a
// flash of the wrong theme; this only handles toggling afterwards.

function toggleTheme() {
    const root = document.documentElement;
    const theme = root.dataset.theme === 'dark' ? 'light' : 'dark';
    root.dataset.theme = theme;
    try {
        localStorage.setItem('ceetcode-theme', theme);
    } catch (e) { /* private mode, storage blocked — theme just won't persist */ }
}

// Follow the OS until the visitor states a preference of their own.
if (window.matchMedia) {
    window.matchMedia('(prefers-color-scheme: light)').addEventListener('change', (e) => {
        let stored = null;
        try {
            stored = localStorage.getItem('ceetcode-theme');
        } catch (err) { /* ignore */ }
        if (!stored) document.documentElement.dataset.theme = e.matches ? 'light' : 'dark';
    });
}


// ─── Copy code (problem pages) ───────────────────────────────────

function copyCode() {
    const lines = document.querySelectorAll('.line-content');
    if (!lines.length) return;
    const code = Array.from(lines).map(l => l.textContent).join('\n');
    const label = document.querySelector('.copy-text');
    const done = (msg) => {
        if (!label) return;
        label.textContent = msg;
        setTimeout(() => { label.textContent = 'copy'; }, 1500);
    };
    navigator.clipboard.writeText(code).then(() => done('copied!'), () => done('failed'));
}


// ─── Index page ──────────────────────────────────────────────────

const table = document.getElementById('problem-table');

if (table) {
    const searchInput = document.getElementById('search');
    const filterBtns = Array.from(document.querySelectorAll('.filter-btn'));
    const noResults = document.getElementById('no-results');
    const resultCount = document.getElementById('result-count');

    const DIFFICULTIES = ['easy', 'medium', 'hard'];

    let problems = [];
    let activeFilter = 'all';

    const escapeHTML = (s) => String(s).replace(/[&<>"']/g, (c) => (
        { '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' }[c]
    ));

    // ── Stats: a total, a stacked difficulty bar, and a legend ──
    function renderStats() {
        const total = problems.length;
        const counts = {};
        DIFFICULTIES.forEach(d => {
            counts[d] = problems.filter(p => p.difficulty === d).length;
        });

        const pct = (n) => (total ? (n / total) * 100 : 0).toFixed(2);
        const segments = DIFFICULTIES
            .map(d => `<span class="stats-seg ${d}" style="--w:${pct(counts[d])}%"></span>`)
            .join('');
        const legend = DIFFICULTIES
            .map(d => `<span class="stat-item ${d}"><span class="stat-value">${counts[d]}</span>
                <span class="stat-label">${d}</span></span>`)
            .join('');

        document.getElementById('stats').innerHTML =
            `<div class="stats-total">
                <span class="stats-total-value">${total}</span>
                <span class="stat-label">solved in c</span>
            </div>
            <div class="stats-bar" role="img" aria-label="${DIFFICULTIES.map(d => `${counts[d]} ${d}`).join(', ')}">${segments}</div>
            <div class="stats-legend">${legend}</div>`;
    }

    // ── Table ──
    function renderTable(list) {
        resultCount.textContent = list.length === problems.length
            ? `${list.length} problems`
            : `${list.length} of ${problems.length}`;

        if (!list.length) {
            table.innerHTML = '';
            noResults.style.display = 'block';
            return;
        }
        noResults.style.display = 'none';

        table.innerHTML = list.map(p => {
            const tags = p.tags.map(t => `<span class="tag">${escapeHTML(t)}</span>`).join('');
            return `<a href="${encodeURIComponent(p.slug)}.html" class="problem-row">
                <span class="problem-num">${escapeHTML(p.number)}</span>
                <span class="problem-title">${escapeHTML(p.title)}</span>
                <span class="problem-tags">${tags}</span>
                <span class="difficulty ${escapeHTML(p.difficulty)}">${escapeHTML(p.difficulty)}</span>
            </a>`;
        }).join('');
    }

    function applyFilters() {
        const q = searchInput.value.toLowerCase().trim();
        renderTable(problems.filter(p => {
            if (activeFilter !== 'all' && p.difficulty !== activeFilter) return false;
            if (!q) return true;
            return p.title.toLowerCase().includes(q) ||
                p.tags.some(t => t.includes(q)) ||
                String(p.number).includes(q);
        }));
    }

    // ── Keyboard: "/" to search, arrows to walk the list, Esc to reset ──
    function rows() {
        return Array.from(table.querySelectorAll('.problem-row'));
    }

    function moveFocus(delta) {
        const all = rows();
        if (!all.length) return;
        const at = all.indexOf(document.activeElement);
        const next = at === -1
            ? (delta > 0 ? 0 : all.length - 1)
            : Math.min(all.length - 1, Math.max(0, at + delta));
        all[next].focus();
    }

    document.addEventListener('keydown', (e) => {
        if (e.metaKey || e.ctrlKey || e.altKey) return;
        const typing = document.activeElement === searchInput;

        if (e.key === '/' && !typing) {
            e.preventDefault();
            searchInput.focus();
            searchInput.select();
            return;
        }
        if (e.key === 'Escape') {
            if (typing && searchInput.value) {
                searchInput.value = '';
                applyFilters();
            } else {
                searchInput.blur();
            }
            return;
        }
        if (e.key === 'ArrowDown' || e.key === 'ArrowUp') {
            if (typing && e.key === 'ArrowUp') return;
            e.preventDefault();
            moveFocus(e.key === 'ArrowDown' ? 1 : -1);
        }
    });

    searchInput.addEventListener('input', applyFilters);

    filterBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            filterBtns.forEach(b => b.setAttribute('aria-pressed', String(b === btn)));
            activeFilter = btn.dataset.filter;
            applyFilters();
        });
    });

    // ── Boot ──
    (async () => {
        try {
            const res = await fetch('./problems.json');
            const data = await res.json();
            problems = data.PROBLEMS || [];
        } catch (err) {
            noResults.textContent = 'could not load problems.';
            noResults.style.display = 'block';
            return;
        }
        renderStats();
        applyFilters();
    })();
}
