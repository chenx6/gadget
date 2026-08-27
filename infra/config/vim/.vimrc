" Basic
syntax on
set nocompatible
set encoding=utf-8
set noswapfile         " don't create swap file
set clipboard+=unnamed " share clipboard
set magic              " regex support
set helplang=cn
" Visual
set number
set cursorline   " highlight current line
set wildmenu     " visual autocomplete
set lazyredraw   " redraw when we need
set showmatch    " highlight matching [{()}]
" Tab and indent
set tabstop=4    " visual of tabs
set expandtab    " tabs are space
set autoindent
set shiftwidth=4 " length of >> and <<
" Search
set incsearch    " search as character are entered
set hlsearch     " highlight search result
set ignorecase   " ignore case when searching
set smartcase    " no ignorecase if Uppercase char present
" Operation
set scrolloff=3  " offset to top/end of buffer

" Key mapping
"   move vertically by visual line
nnoremap j gj
nnoremap k gk
"   move cursor more effectively
nnoremap <C-h> <C-w>h
nnoremap <C-j> <C-w>j
nnoremap <C-k> <C-w>k
nnoremap <C-l> <C-w>l
