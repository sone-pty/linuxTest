syntax on
" 语法高亮

set autoindent
" 自动对齐
 
set confirm
" 在处理未保存或只读文件的时候，弹出确认
 
set tabstop=4
" Tab键的宽度
 
set softtabstop=4
set shiftwidth=4
"  统一缩进为4
 
set noexpandtab
" 不要用空格代替制表符
 
set number
" 显示行号
 
set history=50
" 历史纪录数

set cindent
" 自动缩进

set wildmenu
set magic
set hlsearch            " 高亮显示搜索结果
set wildmenu             " vim自身命名行模式智能补全
set nobackup
set noswapfile
set autoread

"打开文件自动定位到最后编辑的位置
autocmd BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | execute "normal! g'\"" | endif

let mapleader = ","
" 定义跳出括号函数，用于跳出括号
func SkipPair()
    if getline('.')[col('.') - 1] == ')' || getline('.')[col('.') - 1] == ']' || getline('.')[col('.') - 1] == '"' || getline('.')[col('.') - 1] == "'" || getline('.')[col('.') - 1] == '}'
        return "\<ESC>la"
    else
        return "\t"
    endif
endfunc
" 将tab键绑定为跳出括号 
inoremap <TAB> <c-r>=SkipPair()<CR>

nmap <C-l> :w!<CR>
nmap <C-p> :bp!<CR>
nmap <C-n> :bn!<CR>
inoremap ' ''<ESC>i
inoremap " ""<ESC>i
inoremap ( ()<ESC>i
inoremap [ []<ESC>i
inoremap { {<CR>}<ESC>O

"C，C++ 按F5编译运行
map <F5> :call CompileRunGcc()<CR>
func! CompileRunGcc()
	exec "w"
	if &filetype == 'c'
		exec "!g++ % -lpthread -o %<"
		exec "!time ./%<"
	elseif &filetype == 'cpp'
		exec "!g++ *.cpp -std=c++11 -lpthread -o %<"
		exec "!time ./%<"
	elseif &filetype == 'java' 
		exec "!javac %" 
		exec "!time java %<"
	elseif &filetype == 'sh'
		:!time bash %
	elseif &filetype == 'python'
		exec "!time python2.7 %"
    elseif &filetype == 'html'
        exec "!firefox % &"
    elseif &filetype == 'go'
"        exec "!go build %<"
        exec "!time go run %"
    elseif &filetype == 'mkd'
        exec "!~/.vim/markdown.pl % > %.html &"
        exec "!firefox %.html &"
	endif
endfunc

"C,C++的调试
map <F8> :call Rungdb()<CR>
func! Rungdb()
	exec "w"
	exec "rm -f %<"
	exec "!g++ % -std=c++11 -g -o %<"
	exec "!gdb ./%<"
endfunc

"状态栏
set laststatus=2
highlight StatusLine cterm=bold ctermfg=yellow ctermbg=blue
set statusline=%F%m%r%h%w\[POS=%l,%v][%p%%]\[%{&fileformat}]\[%{&encoding}]\[%{strftime(\"%Y-%m-%d\ %H:%M:%S\")}] 
"set statusline=\ %<%F[%1*%M%*%n%R%H]%=\ %y\ %0(%{&fileformat}\ %{&encoding}\ %c:%l/%L%)\ 

nmap <F7> :TlistToggle <CR>

"CTag设置
"set tags+=/usr/include/tags
"set tags+=/usr/include/x86_64-linux-gnu/sys/tags
set tags+=~/.vim/tags/cpp.tags
set tags+=~/.vim/tags/libc.tags
set tags+=~/.vim/tags/glib.tags
set tags+=~/.vim/tags/susv2.tags
set autochdir

"Vundle
set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

" The following are examples of different formats supported.
" Keep Plugin commands between vundle#begin/end.
" plugin on GitHub repo
"Plugin 'tpope/vim-fugitive'
" plugin from http://vim-scripts.org/vim/scripts.html
" Plugin 'L9'
" Git plugin not hosted on GitHub
Plugin 'git://git.wincent.com/command-t.git'
" Install L9 and avoid a Naming conflict if you've already installed a
" different version somewhere else.
" Plugin 'ascenator/L9', {'name': 'newL9'}

" All of your Plugins must be added before the following line
Plugin 'mzlogin/vim-markdown-toc'
Plugin 'Valloric/YouCompleteMe'
Plugin 'octol/vim-cpp-enhanced-highlight'
call vundle#end()            " required
filetype plugin indent on    " required

let g:ycm_global_ycm_extra_conf = '~/.vim/bundle/YouCompleteMe/third_party/ycmd/examples/.ycm_extra_conf.py'
let g:ycm_collect_identifiers_from_tag_files = 1
let g:ycm_confirm_extra_conf=0
let g:ycm_complete_in_comments=1
let g:ycm_error_symbol='✗✗'
let g:ycm_warning_symbol='!!'
let g:ycm_seed_identifiers_with_syntax = 1
set completeopt=longest,menu
set completeopt-=preview
let g:ycm_cache_omnifunc = 0
let g:ycm_key_invoke_completion = '<C-a>'
let g:ycm_min_num_of_chars_for_completion = 2
let g:ycm_key_list_stop_completion = ['<C-z>']
let g:ycm_semantic_triggers =  {
  \   'c' : ['->', '.', 're!\w{2}'],
  \   'objc' : ['->', '.', 're!\[[_a-zA-Z]+\w*\s', 're!^\s*[^\W\d]\w*\s',
  \             're!\[.*\]\s'],
  \   'ocaml' : ['.', '#'],
  \   'cpp,objcpp' : ['->', '.', '::', 're!\w{2}'],
  \   'perl' : ['->'],
  \   'php' : ['->', '::'],
  \   'cs,java,javascript,typescript,d,python,perl6,scala,vb,elixir,go' : ['.'],
  \   'ruby' : ['.', '::'],
  \   'lua' : ['.', ':'],
  \   'erlang' : [':'],
  \ }
nnoremap <c-j> :YcmCompleter GoToDefinitionElseDeclaration<CR>|
