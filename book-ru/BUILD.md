# Сборка

Требуются Pandoc с Lua-фильтрами, XeLaTeX, шрифты Times New Roman, Arial и Consolas; `pdfinfo` для вывода числа страниц. Команды выполняются из корня репозитория.

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File book-ru/build.ps1
```

Скрипт создаёт одиннадцать параграфов, общий PDF 1–11 и обновлённую сокращённую сборку 1–4 в `output/pdf/book-ru/`. Исходники — UTF-8 Markdown; `layout/header.tex` задаёт общую страницу. `layout/context.tex` генерируется при сборке; одновременные запуски сборщика в одной копии не поддерживаются.

Иллюстрации уже сохранены в `figures/`. Для их повторной генерации нужны Python и Pillow:

```powershell
python book-ru/make-figures.py
```

Проверка C++17 (GCC/Clang; без `-DNDEBUG`, поскольку проверки используют `assert`):

```powershell
New-Item -ItemType Directory -Force .local-review/book-ru
g++ -std=c++17 -O2 -Wall -Wextra -pedantic book-ru/code/verify.cpp -o .local-review/book-ru/verify.exe
.local-review/book-ru/verify.exe
```

Каждый заголовок `code/*.hpp` содержит полную реализацию. Листинги в главах могут быть последовательными частями одного класса и не предназначены для компиляции по одному. Временные исполняемые файлы и результаты просмотра страниц остаются в `.local-review/`.

Проверка новых реализаций:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic book-ru/code/verify-05-07.cpp -o .local-review/book-ru/verify-05-07.exe
.local-review/book-ru/verify-05-07.exe
```

`layout/chapter-context.lua` завершает предыдущий параграф перед сменой колонтитула. На оглавлении используется «БГУ · Учебник», на страницах параграфов — их номер и тема. Нумерация N.M хранится в Markdown и одинакова в отдельном и общем PDF.

Проверка §8–10 и генерация их схем:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic book-ru/code/verify-08-10.cpp -o .local-review/book-ru/verify-08-10.exe
.local-review/book-ru/verify-08-10.exe
python book-ru/make-figures-08-10.py
```

Сокращённая сборка 1–7 сохранена и обновлена при исправлении переносов старых глав; текущая общая книга — 1–11. Обычный запуск сборщика пересоздаёт отдельные параграфы, 1–4 и полную книгу.

Полный пример префиксных частот:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic book-ru/code/08-prefix-example.cpp -o .local-review/book-ru/08-prefix-example.exe
.local-review/book-ru/08-prefix-example.exe
```

Ожидаемый вывод — две строки `3` и `3`.

Проверка заключительной главы и генерация её схем:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic book-ru/code/verify-11.cpp -o .local-review/book-ru/verify-11.exe
.local-review/book-ru/verify-11.exe
python book-ru/make-figures-11.py
```

Для быстрой редакционной сборки одной главы вместе с полной книгой:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File book-ru/build.ps1 -Chapter 11
```

Без `-Chapter` пересобираются все отдельные параграфы, сборка 1–4 и полная книга. С параметром пересобираются выбранный параграф и полная книга; остальные файлы сохраняются.

## Подборка iRunner и преподавательский документ

Полная книга 1–11 теперь включает practice/irunner-selection.md после глав. Приложение дополнительно собирается в output/pdf/book-ru/irunner-practice.pdf. Сокращённая книга 1–4 сохраняет только свои главы.

Для сборки всего комплекта, включая отдельные локальные рекомендации преподавателю, выполните:

    powershell -NoProfile -ExecutionPolicy Bypass -File book-ru/build.ps1 -Instructor

Преподавательский источник: assessments/book-ru/practice-instructor.md, выход: output/pdf/book-ru/instructor/practice-instructor.pdf; оба пути от корня репозитория. Ключ -Instructor не добавляет этот документ в студенческую книгу. Обе преподавательские директории исключены из Git. Без локального источника используйте обычную сборку без -Instructor.

Чтобы пересобрать только подборку и преподавательский документ, используйте -PracticeOnly -Instructor. Фильтр layout/keep-practice.lua удерживает каждую карточку преподавательской задачи на одной странице; общий профиль шрифта и полей сохранён.
