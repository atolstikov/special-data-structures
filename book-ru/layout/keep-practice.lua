-- Keep each instructor task card (level-three heading and following blocks) together.
function Pandoc(doc)
  local result = {}
  local open = false
  local function close_card()
    if open then
      table.insert(result, pandoc.RawBlock('latex', '\\end{minipage}\\par\\medskip'))
      open = false
    end
  end
  for _, block in ipairs(doc.blocks) do
    if block.t == 'Header' and block.level <= 3 then
      close_card()
      if block.level == 3 then
        table.insert(result, pandoc.RawBlock('latex', '\\par\\noindent\\begin{minipage}{\\linewidth}'))
        open = true
      end
    end
    table.insert(result, block)
  end
  close_card()
  doc.blocks = result
  return doc
end
