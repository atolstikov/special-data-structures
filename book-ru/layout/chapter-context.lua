-- Clear the previous chapter before changing its running header.
function Header(el)
  if el.level == 1 then
    local title = pandoc.utils.stringify(el.content)
    local latex = pandoc.write(pandoc.Pandoc({pandoc.Plain({pandoc.Str(title)})}), 'latex')
    return {pandoc.RawBlock('latex', '\\clearpage\n\\renewcommand{\\CourseContext}{БГУ · ' .. latex .. '}'), el}
  end
end
