class DSL
  def initialize(&block)
    instance_eval &block if block_given?
  end

  def self.attr_init *vals
    vals.each { |val| class_eval "def #{val} init; @#{val} = init; end" }
    out_string = '#{@' + vals.join('},#{@') + '}'
    class_eval 'def to_s; "' + out_string + '"; end'
  end

  def self.expressions dest, name, *parms

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

puts to_string

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
end
