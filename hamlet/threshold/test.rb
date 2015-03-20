require_relative "threshold.rb"

xxx = query do
  source "Kosh"
  if_ { key 'xyz'; condition 'strcmp(cola, "alpha") == 0)' }
  limit
#  json key: "Molari" 
  csv
end

puts xxx.to_s
