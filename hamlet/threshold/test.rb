require_relative "threshold.rb"

xxx = query do
  source "Kosh"
  if_ { key 'xyz'; condition 'strcmp(cola, "alpha") == 0' }
  limit
  select [ :cola, :colb, :colc ] do
    column_name :cold
  end
  csv  line: 'L', field: 'F' do  
    quote 'Q' 
  end
  isin do
    source "subset"
  end
  part :key => "mykey" do
#    key "mykey"
    command do
      source "Vorlon"
      limit
      isin { source "Shadows" }
      vertical
    end
  end
  index { indexes 1,2,3,5,8,13,21.44,65 }
end

puts xxx.to_s
