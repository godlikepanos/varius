" Vim color file
" Maintainer:	Bram Moolenaar <Bram@vim.org>
" Last Change:	2001 Jul 23

" This is the default color scheme.  It doesn't define the Normal
" highlighting, it uses whatever the colors used to be.

" Set 'background' back to the default.  The value can't always be estimated
" and is then guessed.
hi clear Normal
set bg&

" Remove all existing highlighting and set the defaults.
hi clear

" Load the syntax highlighting defaults, if it's enabled.
if exists("syntax_on")
  syntax reset
endif

let colors_name = "godlike"

hi Directory ctermfg=68 ctermbg=NONE cterm=NONE guifg=#1E90FF guibg=NONE gui=NONE
hi Folded ctermfg=247 ctermbg=16 cterm=NONE guifg=#9a9a9a guibg=#182227 gui=NONE
hi Search ctermfg=NONE ctermbg=NONE cterm=NONE guifg=NONE guibg=#FFFF00 gui=NONE
hi Comment guifg=#00A200 guibg=NONE gui=NONE
hi PreProc guifg=#9C8100 guibg=NONE gui=NONE
hi Number guifg=#1E90FF guibg=NONE gui=NONE
hi Constant guifg=#1E90FF guibg=NONE gui=NONE
hi Type guifg=#ff0000 guibg=NONE gui=NONE
hi Special guifg=#ff0000 guibg=NONE gui=NONE 
hi Statement guifg=#ff0000 guibg=NONE gui=NONE
hi Operator guifg=#ff0000 guibg=NONE gui=NONE
hi LineNr guifg=#000000 guibg=#eeeeee gui=NONE
hi Identifier ctermfg=113 ctermbg=NONE cterm=NONE guifg=#ff0000 guibg=NONE gui=NONE
hi MatchParen guibg=#ffff00

