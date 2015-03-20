require_relative "dsl"

class Threshold < DSL
  attr_init :source
  expressions :out, :CSV, :field, :line, :quote
  expressions :out, :VERTICAL
  expressions :out, :JSON, :key

  expressions :row_terms, :LIMIT, :count, :skip
  expressions :row_terms, :IF_, :key, :condition do
    'if:#{@key},{#{@condition}}'
  end

  def initialize(&block) 
    @row_terms = []
    super &block
  end

  def to_s
    "#{@source} #{@row_terms.join(' ')} #{@out}"
  end
end

def query &block
  Threshold.new &block
end
