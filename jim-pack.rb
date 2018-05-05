#!/usr/bin/env ruby

WORDS = {
  :adverbs => [
    "appropriately",
    "assertively",
    "authoritatively",
    "collaboratively",
    "compellingly",
    "competently",
    "completely",
    "continually",
    "conveniently",
    "credibly",
    "distinctively",
    "dramatically",
    "dynamically",
    "efficiently",
    "energistically",
    "enthusiastically",
    "fungibly",
    "globally",
    "holisticly",
    "interactively",
    "intrinsicly",
    "monotonectally",
    "objectively",
    "phosfluorescently",
    "proactively",
    "professionally",
    "progressively",
    "quickly",
    "rapidiously",
    "seamlessly",
    "synergistically",
    "uniquely",
  ],

  :verbs => [
    "actualize",
    "administrate",
    "aggregate",
    "architect",
    "benchmark",
    "brand",
    "build",
    "cloudify",
    "communicate",
    "conceptualize",
    "coordinate",
    "create",
    "cultivate",
    "customize",
    "deliver",
    "deploy",
    "develop",
    "dinintermediate",
    "disseminate",
    "drive",
    "embrace",
    "e-enable",
    "empower",
    "enable",
    "engage",
    "engineer",
    "enhance",
    "envisioneer",
    "evisculate",
    "evolve",
    "expedite",
    "exploit",
    "extend",
    "fabricate",
    "facilitate",
    "fashion",
    "formulate",
    "foster",
    "generate",
    "grow",
    "harness",
    "ideate",
    "impact",
    "implement",
    "incentivize",
    "incubate",
    "instantiate",
    "initiate",
    "innovate",
    "integrate",
    "iterate",
    "leverage existing",
    "leverage other's",
    "maintain",
    "matrix",
    "maximize",
    "mesh",
    "monetize",
    "morph",
    "myocardinate",
    "negotiate",
    "network",
    "optimize",
    "orchestrate",
    "parallel task",
    "plagiarize",
    "pontificate",
    "predominate",
    "productivate",
    "productize",
    "promote",
    "provide access to",
    "pursue",
    "recaptiualize",
    "reconceptualize",
    "redefine",
    "re-engineer",
    "reintermediate",
    "reinvent",
    "repurpose",
    "restore",
    "revolutionize",
    "scale",
    "seize",
    "simplify",
    "strategize",
    "streamline",
    "supply",
    "syndicate",
    "synergize",
    "synthesize",
    "target",
    "transform",
    "transition",
    "underwhelm",
    "unleash",
    "utilize",
    "visualize",
    "whiteboard",
  ],

  :adjectives => [
    "24/7",
    "24/365",
    "accurate",
    "adaptive",
    "alternative",
    "an expanded array of",
    "B2B",
    "B2C",
    "backend",
    "backward-compatible",
    "best-of-breed",
    "bleeding-edge",
    "bricks-and-clicks",
    "business",
    "clicks-and-mortar",
    "client-based",
    "client-centered",
    "client-centric",
    "client-focused",
    "collaborative",
    "compelling",
    "competitive",
    "cooperative",
    "corporate",
    "cost effective",
    "covalent",
    "cross functional",
    "cross-media",
    "cross-platform",
    "cross-unit",
    "customer directed",
    "customized",
    "cutting-edge",
    "distinctive",
    "distributed",
    "diverse",
    "dynamic",
    "e-business",
    "economically sound",
    "effective",
    "efficient",
    "emerging",
    "empowered",
    "enabled",
    "end-to-end",
    "enterprise",
    "enterprise-wide",
    "equity invested",
    "error-free",
    "ethical",
    "excellent",
    "exceptional",
    "extensible",
    "extensive",
    "flexible",
    "focused",
    "frictionless",
    "front-end",
    "fully researched",
    "fully tested",
    "functional",
    "functionalized",
    "fungible",
    "future-proof",
    "global",
    "go forward",
    "goal-oriented",
    "granular",
    "high standards in",
    "high-payoff",
    "high-quality",
    "highly efficient",
    "holistic",
    "impactful",
    "inexpensive",
    "innovative",
    "installed base",
    "integrated",
    "interactive",
    "interdependent",
    "intermandated",
    "interoperable",
    "intuitive",
    "just in time",
    "leading-edge",
    "leveraged",
    "long-term high-impact",
    "low-risk high-yield",
    "magnetic",
    "maintainable",
    "market positioning",
    "market-driven",
    "mission-critical",
    "multidisciplinary",
    "multifunctional",
    "multimedia based",
    "next-generation",
    "one-to-one",
    "open-source",
    "optimal",
    "orthogonal",
    "out-of-the-box",
    "pandemic",
    "parallel",
    "performance based",
    "plug-and-play",
    "premier",
    "premium",
    "principle-centered",
    "proactive",
    "process-centric",
    "professional",
    "progressive",
    "prospective",
    "quality",
    "real-time",
    "reliable",
    "resource sucking",
    "resource maximizing",
    "resource-leveling",
    "revolutionary",
    "robust",
    "scalable",
    "seamless",
    "stand-alone",
    "standardized",
    "standards compliant",
    "state of the art",
    "sticky",
    "strategic",
    "superior",
    "sustainable",
    "synergistic",
    "tactical",
    "team building",
    "team driven",
    "technically sound",
    "timely",
    "top-line",
    "transparent",
    "turnkey",
    "ubiquitous",
    "unique",
    "user-centric",
    "user friendly",
    "value-added",
    "vertical",
    "viral",
    "virtual",
    "visionary",
    "web-enabled",
    "wireless",
    "world-class",
    "worldwide",
  ],

  :nouns => [
    "action items",
    "alignments",
    "applications",
    "architectures",
    "bandwidth",
    "benefits",
    "best practices",
    "catalysts for change",
    "channels",
    "clouds",
    "collaboration and idea-sharing",
    "communities",
    "content",
    "convergence",
    "core competencies",
    "customer service",
    "data",
    "deliverables",
    "e-business",
    "e-commerce",
    "e-markets",
    "e-tailers",
    "e-services",
    "experiences",
    "expertise",
    "functionalities",
    "fungibility",
    "growth strategies",
    "human capital",
    "ideas",
    "imperatives",
    "infomediaries",
    "information",
    "infrastructures",
    "initiatives",
    "innovation",
    "intellectual capital",
    "interfaces",
    "internal or \"organic\" sources",
    "leadership",
    "leadership skills",
    "manufactured products",
    "markets",
    "materials",
    "meta-services",
    "methodologies",
    "methods of empowerment",
    "metrics",
    "mindshare",
    "models",
    "networks",
    "niches",
    "niche markets",
    "nosql",
    "opportunities",
    "\"outside the box\" thinking",
    "outsourcing",
    "paradigms",
    "partnerships",
    "platforms",
    "portals",
    "potentialities",
    "process improvements",
    "processes",
    "products",
    "quality vectors",
    "relationships",
    "resources",
    "results",
    "ROI",
    "scenarios",
    "schemas",
    "services",
    "solutions",
    "sources",
    "strategic theme areas",
    "storage",
    "supply chains",
    "synergy",
    "systems",
    "technologies",
    "technology",
    "testing procedures",
    "total linkage",
    "users",
    "value",
    "vortals",
    "web-readiness",
    "web services",
  ],

  :joins => %w{
    for
    and
    while
    by
    to
  },

  delims: [
    " ",
    ", ",
    " and ",
    ", and ",
  ],
}

def rw(type, num = 1)
  l = WORDS[type].size
  r = []

  while r.size < num
    v = rand(l)
    r << v unless r.index(v)
  end

  r.map { |i| WORDS[type][i] }
end

def wl(words, j = 'and')
  if words.size > 2
    words[0 .. -2].join(', ') + ", #{j} #{words[-1]}"
  else
    words.join(" #{j} ")
  end
end

def phrase
  ('%s %s %s %s' % [
    wl(rw(:adverbs, rand(2))),
    rw(:verbs).first,
    wl(rw(:adjectives, rand(5))),
    rw(:nouns).first,
  ]).gsub(/\s+/, ' ').strip
end
  
def phrases(num = 1)
  joins = (num > 1) ? rw(:joins, num - 1) : nil

  r = []
  num.times { |i| 
    r.push(joins[i - 1]) if i > 0
    r.push phrase
  }
  
  r.join(' ')
end

def run
  # get number of quotes
  num = (ARGV.shift || '1').to_i

  if num == 1
    puts phrases(1 + rand(3))
  else
    # calculate stats
    stats = num.times.reduce({
      sum: 0,
      min: nil,
      max: nil,
    }) do |r, v| 
      len = phrases(1 + rand(3)).size

      r[:min] = len if !r[:min] || r[:min] > len
      r[:max] = len if !r[:max] || r[:max] < len
      r[:sum] += len

      r
    end

    # print stats
    puts 'ave: %3.1f, num: %d, min: %d, max: %d' % [
      1.0 * stats[:sum] / num,
      num,
      stats[:min],
      stats[:max],
    ]
  end
end

def pack_source
  types = Hash.new { |h, k| h[k] = [] }

  strs = []
  ofs = 0
  max_word_len = 0

  WORDS.each do |type, words|
    words.each do |word|
      len = word.size
      strs << word
      types[type] << { word: word, ofs: ofs, len: len }
      ofs += len
      max_word_len = len if len > max_word_len
    end
  end

  # build combined string
  # (suffix with two spaces to prevent overrun from pgm_read_word())
  all_strs = strs.join('') + '  '
  puts 'static const char TEXT[] PROGMEM =' % [all_strs]

  # print string in blocks of 72 bytes at a time
  # to prevent line overflow in compiler)
  (ofs / 72.0).ceil.times do |i|
    puts '  "%s"' % [all_strs[72 * i, 72].gsub(/"/, '\\"')]
  end
  puts ';'

  # print total text length
  puts '#define TEXT_LEN %d' % [ofs]
  puts ''

  # print word type offset arrays
  types.each do |type, words|
    puts "static const uint16_t %s[] PROGMEM = {\n%s\n};\n#define NUM_%s %d" % [
      type.upcase,
      words.map { |word|
        "  %d, %d, // \"%s\"" % [word[:ofs], word[:len], word[:word]]
      }.join("\n"),
      type.upcase,
      words.size,
    ]
  end

  # print total word count (used to calculate memory use) and
  # maximum word length (used to calculate word buffer size)
  puts '#define TOTAL_WORDS %d' % [types.values.reduce(0) do |r, words|
    r + words.size
  end]

  # print maximum word length (used to calculate word buffer size)
  puts '#define MAX_WORD_LEN %d' % [max_word_len]
end
    
# pack_source
pack_source
