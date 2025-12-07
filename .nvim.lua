local cwd = vim.loop.cwd()
local build_path = cwd..'/build'
require('lspconfig.configs').clang_lsp = {
    default_config = {
        root_dir = cwd,
        cmd = { 'clangd', '--compile-commands-dir='..build_path},
        filetypes = {'cpp', 'hpp', 'c', 'h'},
        autostart = true,
    }
}
require('lspconfig')['clang_lsp'].setup({
    capabilities = require('cmp_nvim_lsp').default_capabilities()
})
-- formatting
vim.api.nvim_create_autocmd(
    {'BufWritePre'},
    {
        pattern = {'*.cpp', '*.c', '*.hpp', '*.h'},
        callback = function(ev)
            vim.lsp.buf.format()
        end
    }
)
