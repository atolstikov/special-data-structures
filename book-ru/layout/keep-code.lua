function CodeBlock(el)
  local n=0
  for _ in el.text:gmatch("[^\n]+") do n=n+1 end
  if n<=50 then
    return {pandoc.RawBlock('latex','\\par\\noindent\\begin{minipage}{\\linewidth}'),el,pandoc.RawBlock('latex','\\end{minipage}\\par')}
  end
end

function OrderedList(el)
  return {pandoc.RawBlock("latex", "\\Needspace{12\\baselineskip}"), el}
end
