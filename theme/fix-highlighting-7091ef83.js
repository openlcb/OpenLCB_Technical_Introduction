// fix-highlighting.js
// Workaround for an mdBook 0.5.2 race condition between highlight.js and mdBook's code block enhancer.
// mdBook sometimes rewrites <pre><code> blocks after highlight.js runs, which strips syntax highlighting.
// This script re-runs highlight.js on all code blocks a few times after page load, so the final DOM
// (after mdBook's enhancer is done) still ends up highlighted.

(function () {
    const MAX_ATTEMPTS = 20;   // stop after this many tries
    const INTERVAL_MS = 250;   // how often to retry (adjust if needed)
    let attempts = 0;

    function rehighlightAll() {
        if (!window.hljs || !window.hljs.highlightBlock) {
            return;
        }

        document.querySelectorAll("pre code").forEach(block => {
            window.hljs.highlightBlock(block);
        });
    }

    function loop() {
        attempts++;
        rehighlightAll();

        if (attempts >= MAX_ATTEMPTS) {
            return; // stop looping
        }

        setTimeout(loop, INTERVAL_MS);
    }

    document.addEventListener("DOMContentLoaded", () => {
        // Start shortly after DOM is ready, so mdBook scripts have run at least once.
        setTimeout(loop, 100);
    });
})();