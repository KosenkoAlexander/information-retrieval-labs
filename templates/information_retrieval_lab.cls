% this file is mostly taken from Tavrov's bachelor thesis class

\ProvidesClass{information_retrieval_lab}[2025/02/06 v0.1 -- Пояснювальна записка до лабораторної роботи з методів організації пошуку інформації]

\LoadClass[a4paper,14pt,ukrainian,oneside]{extreport}

\RequirePackage{cmap}                                   % для кодування шрифтів у pdf
%\RequirePackage[T2A]{fontenc}                           % пакет для підтримки кирилічних шрифтів
%\RequirePackage[utf8]{inputenc}                         % пакет для підтримки текстів у кодуванні UTF-8
\RequirePackage{fontspec}
\RequirePackage{polyglossia}         % пакет для підтримки української та російської мов
\RequirePackage{amssymb,amsfonts,amsmath,amsthm,amscd}  % потрібні символи в режимі математичних формул
\RequirePackage[backend=bibtex,style=numeric]{biblatex} % пакет BibLatex для коректної бібліографії

\setmainlanguage{ukrainian}  % Set main language to Ukrainian
\setotherlanguage{english}  % Set other language to English

\defaultfontfeatures{Ligatures=TeX}  % Enable standard TeX ligatures
\setmainfont{Times New Roman}  % Set main font to Free Serif instead of Times New Roman
\newfontfamily\cyrillicfont{Times New Roman}[Script=Cyrillic]  % Specify font for Cyrillic script
\newfontfamily\cyrillicfonttt{Courier New}

\RequirePackage{upgreek}                                % пакет для прямих грецьких літер
\RequirePackage{textcomp}                               % потрібні символи в режимі математичних формул
\RequirePackage{mathtext}                               % потрібні символи в режимі математичних формул

% пакети для форматування відступів та полів
\RequirePackage[includehead, headsep=0pt, footskip=0pt, top=2cm, bottom=2cm, left=2.5cm, right=1.5cm]{geometry} 
                                                        % задаємо поля сторінки

\RequirePackage{indentfirst}                            % відступ для першого рядка абзацу
\RequirePackage{hanging}                                % відступ для всіх інших рядків абзацу
\RequirePackage[shortlabels]{enumitem}                  % пакет для форматування списків
\RequirePackage{setspace}                               % пакет для того, щоб можна було задати міжрядковий інтервал
\RequirePackage{fancyhdr}                               % пакет потрібний для виставлення позиції номеру сторінки
\RequirePackage{etoolbox}                               % пакет для прибирання висячого відступу в бібліографії та прописування умовних відстаней між заголовками


% графічні пакети та таблиці
\RequirePackage{graphicx}               % основний пакет для вставки графіки
%\RequirePackage{subfig}                 % пакет для вставки підрисунків
\RequirePackage{subcaption}             % пакет для вставки підрисунків (subfig та subcaption конфліктують між собою)

\RequirePackage{flafter}                        % пакет гарантує, що рисунок чи таблиця ніколи не з'являться раніше посилання на самих себе
\RequirePackage[singlelinecheck=off]{caption}   % пакет для персоналізації заголовків рисунків та таблиць 
                                                % (вимкнена перевірка на те, щоб заголовок був у один рядок

%\RequirePackage{xtab}                   % пакет для довгих таблиць
%\RequirePackage{longtable}              % пакет для довгих таблиць
\RequirePackage{multirow}                % пакет для об'єднання рядків таблиці
%\RequirePackage{array}                  % пакет для вирівнювання всередині таблиці
\RequirePackage{tabularx}                % пакет для просунутих таблиць

% стилі заголовків та змісту
\RequirePackage[rm,small,raggedright]{titlesec}         % пакет для ручного керування стилем заголовків
%\RequirePackage[titles,subfigure]{tocloft}              % пакет для ручного керування стилем змісту
\RequirePackage[titles]{tocloft}                        % пакет для ручного керування стилем змісту

\RequirePackage{url}                    % пакет для коректного представлення інтернет-посилань у "Переліку посилань"
\RequirePackage[hidelinks]{hyperref}    % пакет для гіперпосилань у змісті

\RequirePackage{listings}               % пакет для відображення форматування вихідних кодів
\RequirePackage{verbatim}               % пакет для кодів програм

%\RequirePackage{xkeyval}                % пакет для параметричних команд (де я їх використовую?)

\RequirePackage{icomma}                 % пакет для нормальних пробілів після ком у десяткових дробах
\RequirePackage{csquotes}               % для коректної роботи biblatex

\sloppy                 % Позбавляємось від переповнень
\linespread{1.5} % інтервал "1,5"

\newcommand{\setfontsize}[1]{\fontsize{#1pt}{#1pt}\selectfont}  % команда для задавання розміру шрифта
\newcommand\quotes[1]{\guillemotleft#1\guillemotright} % команда для французьких кутових лапок

\urlstyle{same}         % текст гіперпосилань буде того ж шрифту, що й основний текст

% стиль сторінки для позиції номеру сторінки
\newcommand{\headerfooterfont}{\cyrillicfont\selectfont\normalsize}
% стиль сторінки для позиції номеру сторінки
\fancypagestyle{plain}{
    \fancyhf{}
    \rfoot{\headerfooterfont \thepage}
            \renewcommand{\headrulewidth}{0pt}
            \renewcommand{\footrulewidth}{0pt}
}


% команди для заборони використання перенесень
\tolerance=1
\emergencystretch=\maxdimen
\hyphenpenalty=10000
\hbadness=10000

\setlength{\parindent}{1.25cm} % задаємо відступ першого рядка абзацу (5 знаків, або 1,25 см)

\usepackage{xcolor}
\lstdefinestyle{minimalist}{
    basicstyle=\ttfamily\scriptsize,
    commentstyle=\color{gray},
    keywordstyle=\color{black}\bfseries,
    stringstyle=\color{gray},
    numberstyle=\tiny\color{black},
    backgroundcolor=\color{white},
    breaklines=true,
    keepspaces=true,
    numbers=none,
    showstringspaces=false,
    tabsize=2
}

\usepackage{float}
