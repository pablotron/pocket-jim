#!/usr/bin/env ruby

require 'yaml'

module Jim
  class SourcePacker
    def self.run(app, args)
      @app = app
      # get yaml path
      yaml_path = ARGV.shift || File.join(File.dirname(app), 'words.yaml')

      # run packer
      SourcePacker.new(app, yaml_path).run(STDOUT)
    end

    def initialize(app_path, yaml_path)
      @app_path, @yaml_path = app_path, yaml_path
      @data = YAML.load(File.read(yaml_path))
    end

    def run(io)
      types = Hash.new { |h, k| h[k] = { ofs: 0, words: [] } }

      strs = []
      ofs = 0
      max_word_len = 0

      # print header
      io.puts "// generated by #@app_path from #@yaml_path"

      @data.each do |type, words|
        types[type][:ofs] = strs.size
        words.each do |word|
          len = word.size
          strs << word
          types[type][:words] << { word: word, ofs: ofs, len: len }
          ofs += len
          max_word_len = len if len > max_word_len
        end
      end

      # build combined string
      # (suffix with two spaces to prevent overrun from pgm_read_word())
      all_strs = strs.join('') + '  '
      io.puts 'static const char TEXT[] PROGMEM =' % [all_strs]

      # print string in blocks of 72 bytes at a time
      # to prevent line overflow in compiler)
      (ofs / 72.0).ceil.times do |i|
        io.puts '  "%s"' % [all_strs[72 * i, 72].gsub(/"/, '\\"')]
      end
      io.puts ';'

      # print total text length
      io.puts '#define TEXT_LEN %d' % [ofs], ''

      # print word offsets and lengths
      io.puts "static const char * const WORDS[] PROGMEM = {"

      # print offsets
      types.each do |type, type_data|
        io.puts "  // %s\n%s\n\n" % [
          type,
          type_data[:words].map { |word|
            "  %d, %d, // \"%s\"" % [
              word[:ofs],
              word[:len],
              word[:word],
            ]
          }.join("\n"),
        ]
      end

      # close word offsets
      io.puts '};', ''

      types.each do |type, type_data|
        io.puts "#define %s %d\n#define NUM_%s %d\n\n" % [
          type.upcase,
          type_data[:ofs],
          type.upcase,
          type_data[:words].size,
        ]
      end

      # print total word count (used to calculate memory use) and
      # maximum word length (used to calculate word buffer size)
      io.puts '// stats: total word count: %d' % [
        types.values.reduce(0) do |r, type_data|
          r + type_data[:words].size
        end,
      ]

      # print maximum word length (used to calculate word buffer size)
      io.puts '// stats: maximum word length: %d' % [max_word_len]
    end
  end
end
    
# pack_source
Jim::SourcePacker.run($0, ARGV) if __FILE__ == $0
