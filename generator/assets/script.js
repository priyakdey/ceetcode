// ─── Theme ───────────────────────────────────────────────────────
// The initial value is resolved by an inline script in <head> to avoid a
// flash of the wrong theme; this only handles toggling afterwards.

function toggleTheme() {
    const root = document.documentElement;
    const theme = root.dataset.theme === 'dark' ? 'light' : 'dark';
    root.dataset.theme = theme;
    syncGiscusTheme(theme);
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
        if (!stored) {
            const theme = e.matches ? 'light' : 'dark';
            document.documentElement.dataset.theme = theme;
            syncGiscusTheme(theme);
        }
    });
}


// ─── Giscus (problem pages) ──────────────────────────────────────
// Injected here rather than statically so the widget's theme follows the
// site toggle. Built-in transparent/noborder themes let the page
// background show through.

const GISCUS_THEMES = { light: 'noborder_light', dark: 'transparent_dark' };

function syncGiscusTheme(theme) {
    const frame = document.querySelector('iframe.giscus-frame');
    if (!frame) return;
    frame.contentWindow.postMessage(
        { giscus: { setConfig: { theme: GISCUS_THEMES[theme] } } },
        'https://giscus.app'
    );
}

const commentsSection = document.querySelector('.comments-section');
if (commentsSection) {
    const giscus = document.createElement('script');
    giscus.src = 'https://giscus.app/client.js';
    giscus.async = true;
    giscus.crossOrigin = 'anonymous';
    Object.entries({
        'data-repo': 'priyakdey/ceetcode',
        'data-repo-id': 'R_kgDONzIH4g',
        'data-category': 'Comments',
        'data-category-id': 'DIC_kwDONzIH4s4C8a2p',
        'data-mapping': 'pathname',
        'data-strict': '0',
        'data-reactions-enabled': '1',
        'data-emit-metadata': '0',
        'data-input-position': 'top',
        'data-theme': GISCUS_THEMES[document.documentElement.dataset.theme] || GISCUS_THEMES.light,
        'data-lang': 'en',
        'data-loading': 'lazy',
    }).forEach(([k, v]) => giscus.setAttribute(k, v));
    commentsSection.appendChild(giscus);
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

    const DIFFICULTIES = ['easy', 'medium', 'hard'];

    // The toolbar sticks right below the sticky masthead; publish the
    // masthead's height (it varies with viewport width) as a CSS var.
    const masthead = document.querySelector('.masthead');
    const setMastheadHeight = () =>
        document.documentElement.style.setProperty('--masthead-h', `${masthead.offsetHeight}px`);
    setMastheadHeight();
    window.addEventListener('resize', setMastheadHeight);

    let problems = [];
    let activeFilter = 'all';

    const escapeHTML = (s) => String(s).replace(/[&<>"']/g, (c) => (
        { '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' }[c]
    ));

    // ── Stats: total, proportional bar, per-segment counts beneath ──
    function renderStats() {
        const total = problems.length;
        const counts = {};
        DIFFICULTIES.forEach(d => {
            counts[d] = problems.filter(p => p.difficulty === d).length;
        });

        // Bar and label row share flex-grow ratios (--n), so each label
        // lines up under its own segment. Zero-count difficulties are
        // skipped entirely.
        const present = DIFFICULTIES.filter(d => counts[d] > 0);
        const segments = present
            .map(d => `<span class="stats-seg ${d}" style="--n:${counts[d]}"></span>`)
            .join('');
        const labels = present
            .map(d => `<span class="stat-item ${d}" style="--n:${counts[d]}"><span class="stat-value">${counts[d]}</span>
                <span class="stat-label">${d}</span></span>`)
            .join('');

        document.getElementById('stats').innerHTML =
            `<div class="stats-total">
                <span class="stats-total-value">${total}</span>
                <span class="stat-label">problems</span>
            </div>
            <div class="stats-bar" aria-hidden="true">${segments}</div>
            <div class="stats-labels">${labels}</div>`;
    }

    // ── Table ──
    function renderTable(list) {
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
