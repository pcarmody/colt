class DSL
  def initialize(&block)
    instance_eval &block if block_given?
  end

  def self.attr_init *vals
    vals.each { |val| class_eval "def #{val} init; @#{val} = init; end" }
    out_string = '#{@' + vals.join('},#{@') + '}'
    class_eval 'def to_s; "' + out_string + '"; end'
  end

  def self.attr_dsl name
    class_eval %Q{
  def #{name} &block
    @#{name}= Nested.new(nil, &block)
  end
}
  end

  def self.attr_list name
    class_eval %Q{
  def #{name} *parms
    @#{name} ||= []
    parms.each { |p| @#{name} << p }
  end
}
  end

  def self.attr_nested dest, name
    class_eval %Q{
  def #{name} &block
    retval = Nested.new("#{name}", &block)
    if !@#{dest}
      @#{dest} = retval
    else
      @#{dest} << retval
    end
    retval
  end
}
  end

  def self.expression dest, name, *parms

    init = ''
    init << "attr_init :" << parms.join(", :") if parms.size > 0

    if block_given?
      to_string = '"' << yield << '"'
    else
      to_string = %Q{retval = '#{name.downcase}'; }
      if parms.size > 0
        to_string << 'retval << ":#{@' + parms.join('},#{@') + '}"' 
        to_string << " if @#{parms.join(' || @')}; "
      end
      to_string << "return retval;"
    end

   class_eval %Q{ 
class #{name} < DSL; 
  #{init}; 
  def to_s
     #{to_string}
  end
end 
}

    class_eval %Q{
  def #{name.downcase} options = {}, &block
    retval = #{name}.new(&block)
    options.each_pair { | k, v | retval.send(k,v) } if options.size > 0
    if !@#{dest}
      @#{dest} = retval
    else
      @#{dest} << retval
    end
  end 
}

  end

  def self.list_expression dest, name, list

    class_eval %Q{
  class #{name} < DSL 
    def initialize cols=[], &block
      @#{list} = cols
      super &block
    end
    def #{list} val
      @#{list} ||= []
      @#{list} << val
    end
    def to_s
       'select:' + @#{list}.join(",")
    end
  end 
}

    class_eval %Q{
  def select cols, &block
    retval = #{name}.new(cols, &block)
#    options.each_pair { | k, v | retval.send(k,v) } if options.size > 0
    if !@#{dest}
      @#{dest} = retval
    else
      @#{dest} << retval
    end
  end 
}

  end

  def self.expression_with_parms dest, name, parms={}

    init = ''
    if parms.size > 0 
      parms.each_pair do | name, type |
        if type == :string
          init << "  attr_init :#{name}\n"
        elsif type == :nested
          init << "  attr_dsl :#{name}\n"
        elsif type == :list
          init << "  attr_list :#{name}\n"
        else
        end
      end
    end

    if block_given?
      to_string = '"' << yield << '"'
    else
      to_string = %Q{retval = '#{name.downcase}'; }
      if parms.size > 0
        to_string << 'retval << ":#{@' + parms.keys.join('},#{@') + '}"' 
        to_string << " if @#{parms.keys.join(' || @')}; "
      end
      to_string << "return retval;"
    end

   xxx = %Q{ 
class #{name} < Threshold
  #{init} 
  def to_s
     #{to_string}
  end
end 
}
puts xxx
    class_eval xxx

    class_eval %Q{
  def #{name.downcase} options = {}, &block
    retval = #{name}.new(&block)
    options.each_pair { | k, v | retval.send(k,v) } if options.size > 0
    if !@#{dest}
      @#{dest} = retval
    else
      @#{dest} << retval
    end
  end 
}

  end

end
