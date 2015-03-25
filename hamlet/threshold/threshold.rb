require_relative "dsl"

class Threshold < DSL
  attr_init :source
  expression_with_parms :out, :CSV, :field => :string, :line => :string, :quote => :string
#  expression :out, :CSV, :field, :line, :quote
  expression :out, :VERTICAL
  expression :out, :JSON, :key

  expression :row_terms, :LIMIT, :count, :skip
  expression :row_terms, :IF_, :key, :condition do
    'if:#{@key},{#{@condition}}'
  end
  expression :row_terms, :RACE, :flags, :left_column, :right_source, :right_column

  list_expression :thru_terms, :SORT, :column_name
  expression :thru_terms, :SEARCH, :low, :high

  list_expression :column_terms, :SELECT, :column_name
  list_expression :column_terms, :IGNORE, :column_name

 expression_with_parms :row_terms, :PART, :key => :string, :command => :nested
 expression_with_parms :row_terms, :INDEX, :indexes => :list, :file_name => :string do
    'index:#{@indexes.join(",")}:#{@file_name}'
  end

  def initialize(&block) 
    @row_terms = []
    @column_terms = []
    super &block
  end

  class Nested < Threshold
    def initialize n, &block
      @name = n
      super &block
    end
  
    def to_s
      @name ?  "#{@name}:[#{super}]" : "[#{super}]" 
    end
  end

  attr_nested :row_terms, :isin

  def to_s
    "#{@source} #{@row_terms.join(' ')} #{@column_terms.join(' ')} #{@out}"
  end
end

def query &block
  Threshold.new &block
end
