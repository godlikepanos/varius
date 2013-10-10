set nocompatible
"match ErrorMsg /\%>80v.\+/
set guifont=Monospace\ 9
set nu
set ai
set cin
"set smartindent
set colorcolumn=81

if has('gui_running')
	set columns=85
	set lines=70
	set numberwidth=4
	"match ErrorMsg /\s\+$/ " Highlight tailing spaces
endif

" No .swp file
:set noswapfile

" Tabs
set tabstop=4
set shiftwidth=4
set noexpandtab

" Highlight sceme
colorscheme godlike

" CTRL+C and other
source $VIMRUNTIME/mswin.vim
behave mswin

" Highlight tabs with F10
let g:highlighttabs=0
function HighlightTabs()
	if g:highlighttabs == 0
		let g:highlighttabs = 1
		set list listchars=tab:>-,trail:.
	else
		let g:highlighttabs = 0
		set list!
	endif
endfunction
nmap <F10> mz:execute HighlightTabs()<CR>'z

" Double columns with F11
function DoubleColumns()
	let tmp=&columns
	let &columns=tmp*2+1
endfunction
nmap <F11> mz:execute DoubleColumns()<CR>'z
	
autocmd VimResized * wincmd =

" Change the margin between 80 and 120 columns
function SetMargin()
	if &colorcolumn == 81
		set colorcolumn=121
	else
		set colorcolumn=81
	endif
endfunction
nmap <C-F11> mz:execute SetMargin()<CR>'z

" Bring up NERDTree F9
let g:NERDTreeDirArrows = 0
nmap <silent> <F9> :NERDTreeToggle<CR>:let &columns=&columns+40<CR>

" Minibuff explorer config
let g:miniBufExplMapWindowNavVim = 1
let g:miniBufExplMapWindowNavArrows = 1
let g:miniBufExplMapCTabSwitchBufs = 1
let g:miniBufExplModSelTarget = 1 

" Buffers navigation with CTRL+<something>
nmap <silent> <C-PageUp> :bprev<CR>
imap <silent> <C-PageUp> <Esc>:bprev<CR>i

nmap <silent> <C-PageDown> :bnext<CR>
imap <silent> <C-PageDown> <Esc>:bnext<CR>i

nmap <silent> <C-w> :bd<CR>
imap <silent> <C-w> <Esc>:bd<CR>i

" Highlight search
set hlsearch

" GLSL
command SetGLSLFileType call SetGLSLFileType()
function SetGLSLFileType()
	execute ':set filetype=glsl'
endfunction
au BufNewFile,BufRead *.{frag,vert,fp,vp,glsl} SetGLSLFileType

" Tag list config and pap F12
let Tlist_Ctags_Cmd = "/usr/bin/ctags"
let Tlist_WinWidth = 60
let Tlist_Use_Right_Window = 1
map <F12> :TlistToggle<cr>
map <C-F12> :!/usr/bin/ctags -R --c++-kinds=+p --fields=+iaS --extra=+fq .<CR>

" Move nicely on wrapped lines
"imap <silent> <Down> <C-o>gj
"imap <silent> <Up> <C-o>gk
"nmap <silent> <Down> gj
"nmap <silent> <Up> gk

" Remove the toolbar
set guioptions-=T

" Open header file under curson with F8
nmap <silent> <F8> :e **/<cfile><cr>

" FuzzyFinder plugin config
nnoremap <C-l> :FufBuffer<CR>
nnoremap <C-o> :FufFile<CR>

inoremap <C-l> <Esc>:FufBuffer<CR>
inoremap <C-o> <Esc>:FufFile<CR>

" Move between splits with ALT and arrows
nnoremap <silent> <A-Up> :wincmd k<CR>
nnoremap <silent> <A-Down> :wincmd j<CR>
nnoremap <silent> <A-Left> :wincmd h<CR>
nnoremap <silent> <A-Right> :wincmd l<CR>

inoremap <silent> <A-Up> <Esc>:wincmd k<CR>
inoremap <silent> <A-Down> <Esc>:wincmd j<CR>
inoremap <silent> <A-Left> <Esc>:wincmd h<CR>
inoremap <silent> <A-Right> <Esc>:wincmd l<CR>

" Switch between header and source
function! HeaderSourceSwitch()
	if(expand("%:e") == "cpp")
		execute ':tag ' . expand("%:t:r") . '.h'
	else
		execute ':tag ' . expand("%:t:r") . '.cpp'
	endif
endfunction

nnoremap <silent> <C-Tab> :call HeaderSourceSwitch()<CR>
inoremap <silent> <C-Tab> <Esc>:call HeaderSourceSwitch()<CR>
