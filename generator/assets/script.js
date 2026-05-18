// ─── init ───────────────────────────────────────────────
(async () => {
    const res = await fetch("./problems.json");
    const data = await res.json();

    const PROBLEMS = data["PROBLEMS"];


    renderStats(PROBLEMS);
    renderTable(PROBLEMS);

    searchInput.addEventListener('input', () => applyFilters(PROBLEMS));

    filterBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            filterBtns.forEach(b => b.classList.remove('active'));
            btn.classList.add('active');
            activeFilter = btn.dataset.filter;
            applyFilters(PROBLEMS);
        });
    });
})();

// ─── Render stats ────────────────────────────────────────────────
function renderStats(PROBLEMS) {
    const total = PROBLEMS.length;
    const easy = PROBLEMS.filter(p => p.difficulty === 'easy').length;
    const medium = PROBLEMS.filter(p => p.difficulty === 'medium').length;
    const hard = PROBLEMS.filter(p => p.difficulty === 'hard').length;

    document.getElementById('stats').innerHTML =
        stat(total, 'problems') + stat(easy, 'easy') +
        stat(medium, 'medium') + stat(hard, 'hard');
}

function stat(value, label) {
    return `<div class="stat-item">
            <span class="stat-value">${value}</span>
            <span class="stat-label">${label}</span>
        </div>`;
}

// ─── Render table ────────────────────────────────────────────────
function renderTable(problems) {
    const table = document.getElementById('problem-table');
    const noResults = document.getElementById('no-results');

    if (problems.length === 0) {
        table.innerHTML = '';
        noResults.style.display = 'block';
        return;
    }

    noResults.style.display = 'none';
    table.innerHTML = problems.map(p => {
        const num = p.number; 
        const tags = p.tags.map(t => `<span class="tag">${t}</span>`).join('');
        return `<a href="${p.slug}.html" class="problem-row">
                <span class="problem-num">${num}</span>
                <span class="problem-title">${p.title}</span>
                <span class="problem-tags">${tags}</span>
                <span class="difficulty ${p.difficulty}">${capitalize(p.difficulty)}</span>
            </a>`;
    }).join('');
}

function capitalize(s) { return s.charAt(0).toUpperCase() + s.slice(1); }

// ─── Search & Filter ─────────────────────────────────────────────
const searchInput = document.getElementById('search');
const filterBtns = document.querySelectorAll('.filter-btn');
let activeFilter = 'all';

function applyFilters(PROBLEMS) {
    const q = searchInput.value.toLowerCase().trim();
    const filtered = PROBLEMS.filter(p => {
        const matchesFilter = activeFilter === 'all' || p.difficulty === activeFilter;
        if (!matchesFilter) return false;
        if (!q) return true;
        return p.title.toLowerCase().includes(q) ||
            p.tags.some(t => t.includes(q)) ||
            String(p.number).includes(q);
    });
    renderTable(filtered);
}


// ─── Theme ───────────────────────────────────────────────────────
function toggleTheme() {
    const html = document.documentElement;
    const theme = html.dataset.theme === 'dark' ? 'light' : 'dark';
    html.dataset.theme = theme;
    localStorage.setItem('ceetcode-theme', theme);
}

const saved = localStorage.getItem('ceetcode-theme');
if (saved) document.documentElement.dataset.theme = saved;


// ─── Copy code ───────────────────────────────────────────────────────
function copyCode() {
    const lines = document.querySelectorAll('.line-content');
    const code = Array.from(lines).map(l => l.textContent).join('\n');
    navigator.clipboard.writeText(code).then(() => {
        const textEl = document.querySelector('.copy-text');
        textEl.textContent = 'copied!';
        setTimeout(() => textEl.textContent = 'copy', 1500);
    });
}
