#!/bin/bash

# install vim-plug
curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim



# put plugins in vim config

cat << EOF > ~/.vimrc

call plug#begin('~/.vim/plugged')

Plug 'joshdick/onedark.vim'
Plug 'ghifarit53/tokyonight-vim'
Plug 'jiangmiao/auto-pairs'
Plug 'sheerun/vim-polyglot'
Plug 'itchyny/lightline.vim'


call plug#end()

EOF



# tell vim to install the plugins

vim -c PlugInstall -c :q -c :q



# add config

cat << EOF >> ~/.vimrc

let g:tokyonight_style = 'night'
let g:tokyonight_enable_italic = 0

let g:lightline = { 'colorscheme': 'tokyonight' }

syntax on
set termguicolors
colorscheme tokyonight



set hidden
set nu rnu
set tabstop=4
set shiftwidth=4
set expandtab
set mouse=a
set clipboard=unnamedplus
set laststatus=2

EOF
