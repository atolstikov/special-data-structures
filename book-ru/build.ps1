param([ValidateRange(1,11)][int]$Chapter, [switch]$Instructor, [switch]$PracticeOnly)
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$out = Join-Path $root '../output/pdf/book-ru'
New-Item -ItemType Directory -Force -Path $out | Out-Null
Push-Location $root
try {
    $chapters = @(Get-ChildItem 'chapters/*.md' | Sort-Object Name)
    [IO.File]::WriteAllText((Join-Path $root 'layout/context.tex'),
        "\newcommand{\CourseContext}{БГУ · Учебник}`n",[Text.UTF8Encoding]::new($false))
    $argsBase = @('--from','markdown+tex_math_dollars','--resource-path','.',
        '--pdf-engine','xelatex','--include-in-header','layout/header.tex',
        '--include-in-header','layout/context.tex',
        '--lua-filter','layout/keep-code.lua','--lua-filter','layout/chapter-context.lua',
        '-V','documentclass:article','-V','fontsize:11pt','-V','papersize:a4',
        '-V','mainfont:Times New Roman','-V','sansfont:Arial',
        '-V','monofont:Consolas','-V','monofontoptions:Scale=0.85')
    $individual = $chapters
    if ($PSBoundParameters.ContainsKey('Chapter')) {
        $individual = @($chapters | Where-Object { $_.Name -match ('^{0:D2}-' -f $Chapter) })
        if ($individual.Count -ne 1) { throw "Chapter source not found: $Chapter" }
    }
    if ($PracticeOnly) { $individual = @() }
    foreach ($chapterFile in $individual) {
        & pandoc $chapterFile.FullName @argsBase -o (Join-Path $out ($chapterFile.BaseName+'.pdf'))
        if ($LASTEXITCODE) { throw "Failed: $chapterFile" }
    }
    $counts = @(4,$chapters.Count)
    if ($PSBoundParameters.ContainsKey('Chapter')) { $counts = @($chapters.Count) }
    if ($PracticeOnly) { $counts = @() }
    foreach ($count in $counts | Select-Object -Unique) {
        $selected = @($chapters | Select-Object -First $count)
        $name = 'sds-ru-weeks-01-{0:D2}.pdf' -f $count
        $inputs = @($selected.FullName)
        if ($count -eq $chapters.Count) { $inputs += (Join-Path $root 'practice/irunner-selection.md') }
        & pandoc @inputs @argsBase --toc --toc-depth=2 -V 'toc-title:Содержание' -o (Join-Path $out $name)
        if ($LASTEXITCODE) { throw "Combined book failed: $name" }
    }
    & pandoc 'practice/irunner-selection.md' @argsBase -o (Join-Path $out 'irunner-practice.pdf')
    if ($LASTEXITCODE) { throw 'Practice catalogue build failed' }
    if ($Instructor) {
        $privateOut = Join-Path $out 'instructor'
        New-Item -ItemType Directory -Force -Path $privateOut | Out-Null
        & pandoc '../assessments/book-ru/practice-instructor.md' @argsBase --lua-filter 'layout/keep-practice.lua' -o (Join-Path $privateOut 'practice-instructor.pdf')
        if ($LASTEXITCODE) { throw 'Instructor guide build failed' }
    }
    Get-ChildItem $out -Filter '*.pdf' | ForEach-Object { $_.Name; & pdfinfo $_.FullName | Select-String '^Pages:|^Page size:' }
} finally { Pop-Location }
