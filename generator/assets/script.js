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
// The stats block and problem table are rendered into the HTML at build
// time (crawlers need real links); this script only filters the rows.

const table = document.getElementById('problem-table');

if (table) {
    const searchInput = document.getElementById('search');
    const filterBtns = Array.from(document.querySelectorAll('.filter-btn'));
    const noResults = document.getElementById('no-results');

    // The toolbar sticks right below the sticky masthead; publish the
    // masthead's height (it varies with viewport width) as a CSS var.
    const masthead = document.querySelector('.masthead');
    const setMastheadHeight = () =>
        document.documentElement.style.setProperty('--masthead-h', `${masthead.offsetHeight}px`);
    setMastheadHeight();
    window.addEventListener('resize', setMastheadHeight);

    const items = Array.from(table.querySelectorAll('.problem-row')).map(row => ({
        row,
        number: row.querySelector('.problem-num').textContent.trim(),
        title: row.querySelector('.problem-title').textContent.trim().toLowerCase(),
        tags: Array.from(row.querySelectorAll('.tag')).map(t => t.textContent.toLowerCase()),
        difficulty: row.dataset.difficulty,
    }));

    let activeFilter = 'all';

    function applyFilters() {
        const q = searchInput.value.toLowerCase().trim();
        let visible = 0;
        items.forEach(({ row, number, title, tags, difficulty }) => {
            const show =
                (activeFilter === 'all' || difficulty === activeFilter) &&
                (!q || title.includes(q) || tags.some(t => t.includes(q)) || number.includes(q));
            row.style.display = show ? '' : 'none';
            if (show) visible += 1;
        });
        noResults.style.display = visible ? 'none' : 'block';
    }

    // ── Keyboard: "/" to search, arrows to walk the list, Esc to reset ──
    function rows() {
        return items.filter(it => it.row.style.display !== 'none').map(it => it.row);
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
}
