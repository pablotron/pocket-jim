#include "jim.h"
#ifdef __AVR_ARCH__
#include <avr/pgmspace.h> // for PROGMEM, memcpy_P
#define GET_WORD_OFFSET(words, id) pgm_read_word_near((words) + (2 * (id) + 0))
#define GET_WORD_LENGTH(words, id) pgm_read_word_near((words) + (2 * (id) + 1))
#else /* !__AVR_ARCH__ */
#include <string.h> // memcpy()
#define PROGMEM
#define memcpy_P memcpy
#define GET_WORD_OFFSET(words, id) words[2 * (id) + 0]
#define GET_WORD_LENGTH(words, id) words[2 * (id) + 1]
#endif /* __AVR_ARCH__ */

typedef enum {
  DELIM_SPACE,
  DELIM_COMMA,
  DELIM_AND,
  DELIM_COMMA_AND,
  DELIM_LAST,
} delim_t;

static const char TEXT[] PROGMEM =
  "appropriatelyassertivelyauthoritativelycollaborativelycompellinglycompet"
  "entlycompletelycontinuallyconvenientlycrediblydistinctivelydramaticallyd"
  "ynamicallyefficientlyenergisticallyenthusiasticallyfungiblygloballyholis"
  "ticlyinteractivelyintrinsiclymonotonectallyobjectivelyphosfluorescentlyp"
  "roactivelyprofessionallyprogressivelyquicklyrapidiouslyseamlesslysynergi"
  "sticallyuniquelyactualizeadministrateaggregatearchitectbenchmarkbrandbui"
  "ldcloudifycommunicateconceptualizecoordinatecreatecultivatecustomizedeli"
  "verdeploydevelopdinintermediatedisseminatedriveembracee-enableempowerena"
  "bleengageengineerenhanceenvisioneerevisculateevolveexpediteexploitextend"
  "fabricatefacilitatefashionformulatefostergenerategrowharnessideateimpact"
  "implementincentivizeincubateinstantiateinitiateinnovateintegrateiteratel"
  "everage existingleverage other'smaintainmatrixmaximizemeshmonetizemorphm"
  "yocardinatenegotiatenetworkoptimizeorchestrateparallel taskplagiarizepon"
  "tificatepredominateproductivateproductizepromoteprovide access topursuer"
  "ecaptiualizereconceptualizeredefinere-engineerreintermediatereinventrepu"
  "rposerestorerevolutionizescaleseizesimplifystrategizestreamlinesupplysyn"
  "dicatesynergizesynthesizetargettransformtransitionunderwhelmunleashutili"
  "zevisualizewhiteboard24/724/365accurateadaptivealternativean expanded ar"
  "ray ofB2BB2Cbackendbackward-compatiblebest-of-breedbleeding-edgebricks-a"
  "nd-clicksbusinessclicks-and-mortarclient-basedclient-centeredclient-cent"
  "ricclient-focusedcollaborativecompellingcompetitivecooperativecorporatec"
  "ost effectivecovalentcross functionalcross-mediacross-platformcross-unit"
  "customer directedcustomizedcutting-edgedistinctivedistributeddiversedyna"
  "mice-businesseconomically soundeffectiveefficientemergingempoweredenable"
  "dend-to-endenterpriseenterprise-wideequity investederror-freeethicalexce"
  "llentexceptionalextensibleextensiveflexiblefocusedfrictionlessfront-endf"
  "ully researchedfully testedfunctionalfunctionalizedfungiblefuture-proofg"
  "lobalgo forwardgoal-orientedgranularhigh standards inhigh-payoffhigh-qua"
  "lityhighly efficientholisticimpactfulinexpensiveinnovativeinstalled base"
  "integratedinteractiveinterdependentintermandatedinteroperableintuitiveju"
  "st in timeleading-edgeleveragedlong-term high-impactlow-risk high-yieldm"
  "agneticmaintainablemarket positioningmarket-drivenmission-criticalmultid"
  "isciplinarymultifunctionalmultimedia basednext-generationone-to-oneopen-"
  "sourceoptimalorthogonalout-of-the-boxpandemicparallelperformance basedpl"
  "ug-and-playpremierpremiumprinciple-centeredproactiveprocess-centricprofe"
  "ssionalprogressiveprospectivequalityreal-timereliableresource suckingres"
  "ource maximizingresource-levelingrevolutionaryrobustscalableseamlessstan"
  "d-alonestandardizedstandards compliantstate of the artstickystrategicsup"
  "eriorsustainablesynergistictacticalteam buildingteam driventechnically s"
  "oundtimelytop-linetransparentturnkeyubiquitousuniqueuser-centricuser fri"
  "endlyvalue-addedverticalviralvirtualvisionaryweb-enabledwirelessworld-cl"
  "assworldwideaction itemsalignmentsapplicationsarchitecturesbandwidthbene"
  "fitsbest practicescatalysts for changechannelscloudscollaboration and id"
  "ea-sharingcommunitiescontentconvergencecore competenciescustomer service"
  "datadeliverablese-businesse-commercee-marketse-tailerse-servicesexperien"
  "cesexpertisefunctionalitiesfungibilitygrowth strategieshuman capitalidea"
  "simperativesinfomediariesinformationinfrastructuresinitiativesinnovation"
  "intellectual capitalinterfacesinternal or \"organic\" sourcesleadershiplea"
  "dership skillsmanufactured productsmarketsmaterialsmeta-servicesmethodol"
  "ogiesmethods of empowermentmetricsmindsharemodelsnetworksnichesniche mar"
  "ketsnosqlopportunities\"outside the box\" thinkingoutsourcingparadigmspart"
  "nershipsplatformsportalspotentialitiesprocess improvementsprocessesprodu"
  "ctsquality vectorsrelationshipsresourcesresultsROIscenariosschemasservic"
  "essolutionssourcesstrategic theme areasstoragesupply chainssynergysystem"
  "stechnologiestechnologytesting procedurestotal linkageusersvaluevortalsw"
  "eb-readinessweb servicesforandwhilebyto ,  and , and   "
;
#define TEXT_LEN 4013

static const uint16_t ADVERBS[] PROGMEM = {
  0, 13, // "appropriately"
  13, 11, // "assertively"
  24, 15, // "authoritatively"
  39, 15, // "collaboratively"
  54, 12, // "compellingly"
  66, 11, // "competently"
  77, 10, // "completely"
  87, 11, // "continually"
  98, 12, // "conveniently"
  110, 8, // "credibly"
  118, 13, // "distinctively"
  131, 12, // "dramatically"
  143, 11, // "dynamically"
  154, 11, // "efficiently"
  165, 14, // "energistically"
  179, 16, // "enthusiastically"
  195, 8, // "fungibly"
  203, 8, // "globally"
  211, 10, // "holisticly"
  221, 13, // "interactively"
  234, 11, // "intrinsicly"
  245, 14, // "monotonectally"
  259, 11, // "objectively"
  270, 17, // "phosfluorescently"
  287, 11, // "proactively"
  298, 14, // "professionally"
  312, 13, // "progressively"
  325, 7, // "quickly"
  332, 11, // "rapidiously"
  343, 10, // "seamlessly"
  353, 15, // "synergistically"
  368, 8, // "uniquely"
};
#define NUM_ADVERBS 32
static const uint16_t VERBS[] PROGMEM = {
  376, 9, // "actualize"
  385, 12, // "administrate"
  397, 9, // "aggregate"
  406, 9, // "architect"
  415, 9, // "benchmark"
  424, 5, // "brand"
  429, 5, // "build"
  434, 8, // "cloudify"
  442, 11, // "communicate"
  453, 13, // "conceptualize"
  466, 10, // "coordinate"
  476, 6, // "create"
  482, 9, // "cultivate"
  491, 9, // "customize"
  500, 7, // "deliver"
  507, 6, // "deploy"
  513, 7, // "develop"
  520, 15, // "dinintermediate"
  535, 11, // "disseminate"
  546, 5, // "drive"
  551, 7, // "embrace"
  558, 8, // "e-enable"
  566, 7, // "empower"
  573, 6, // "enable"
  579, 6, // "engage"
  585, 8, // "engineer"
  593, 7, // "enhance"
  600, 11, // "envisioneer"
  611, 10, // "evisculate"
  621, 6, // "evolve"
  627, 8, // "expedite"
  635, 7, // "exploit"
  642, 6, // "extend"
  648, 9, // "fabricate"
  657, 10, // "facilitate"
  667, 7, // "fashion"
  674, 9, // "formulate"
  683, 6, // "foster"
  689, 8, // "generate"
  697, 4, // "grow"
  701, 7, // "harness"
  708, 6, // "ideate"
  714, 6, // "impact"
  720, 9, // "implement"
  729, 11, // "incentivize"
  740, 8, // "incubate"
  748, 11, // "instantiate"
  759, 8, // "initiate"
  767, 8, // "innovate"
  775, 9, // "integrate"
  784, 7, // "iterate"
  791, 17, // "leverage existing"
  808, 16, // "leverage other's"
  824, 8, // "maintain"
  832, 6, // "matrix"
  838, 8, // "maximize"
  846, 4, // "mesh"
  850, 8, // "monetize"
  858, 5, // "morph"
  863, 12, // "myocardinate"
  875, 9, // "negotiate"
  884, 7, // "network"
  891, 8, // "optimize"
  899, 11, // "orchestrate"
  910, 13, // "parallel task"
  923, 10, // "plagiarize"
  933, 11, // "pontificate"
  944, 11, // "predominate"
  955, 12, // "productivate"
  967, 10, // "productize"
  977, 7, // "promote"
  984, 17, // "provide access to"
  1001, 6, // "pursue"
  1007, 13, // "recaptiualize"
  1020, 15, // "reconceptualize"
  1035, 8, // "redefine"
  1043, 11, // "re-engineer"
  1054, 14, // "reintermediate"
  1068, 8, // "reinvent"
  1076, 9, // "repurpose"
  1085, 7, // "restore"
  1092, 13, // "revolutionize"
  1105, 5, // "scale"
  1110, 5, // "seize"
  1115, 8, // "simplify"
  1123, 10, // "strategize"
  1133, 10, // "streamline"
  1143, 6, // "supply"
  1149, 9, // "syndicate"
  1158, 9, // "synergize"
  1167, 10, // "synthesize"
  1177, 6, // "target"
  1183, 9, // "transform"
  1192, 10, // "transition"
  1202, 10, // "underwhelm"
  1212, 7, // "unleash"
  1219, 7, // "utilize"
  1226, 9, // "visualize"
  1235, 10, // "whiteboard"
};
#define NUM_VERBS 99
static const uint16_t ADJECTIVES[] PROGMEM = {
  1245, 4, // "24/7"
  1249, 6, // "24/365"
  1255, 8, // "accurate"
  1263, 8, // "adaptive"
  1271, 11, // "alternative"
  1282, 20, // "an expanded array of"
  1302, 3, // "B2B"
  1305, 3, // "B2C"
  1308, 7, // "backend"
  1315, 19, // "backward-compatible"
  1334, 13, // "best-of-breed"
  1347, 13, // "bleeding-edge"
  1360, 17, // "bricks-and-clicks"
  1377, 8, // "business"
  1385, 17, // "clicks-and-mortar"
  1402, 12, // "client-based"
  1414, 15, // "client-centered"
  1429, 14, // "client-centric"
  1443, 14, // "client-focused"
  1457, 13, // "collaborative"
  1470, 10, // "compelling"
  1480, 11, // "competitive"
  1491, 11, // "cooperative"
  1502, 9, // "corporate"
  1511, 14, // "cost effective"
  1525, 8, // "covalent"
  1533, 16, // "cross functional"
  1549, 11, // "cross-media"
  1560, 14, // "cross-platform"
  1574, 10, // "cross-unit"
  1584, 17, // "customer directed"
  1601, 10, // "customized"
  1611, 12, // "cutting-edge"
  1623, 11, // "distinctive"
  1634, 11, // "distributed"
  1645, 7, // "diverse"
  1652, 7, // "dynamic"
  1659, 10, // "e-business"
  1669, 18, // "economically sound"
  1687, 9, // "effective"
  1696, 9, // "efficient"
  1705, 8, // "emerging"
  1713, 9, // "empowered"
  1722, 7, // "enabled"
  1729, 10, // "end-to-end"
  1739, 10, // "enterprise"
  1749, 15, // "enterprise-wide"
  1764, 15, // "equity invested"
  1779, 10, // "error-free"
  1789, 7, // "ethical"
  1796, 9, // "excellent"
  1805, 11, // "exceptional"
  1816, 10, // "extensible"
  1826, 9, // "extensive"
  1835, 8, // "flexible"
  1843, 7, // "focused"
  1850, 12, // "frictionless"
  1862, 9, // "front-end"
  1871, 16, // "fully researched"
  1887, 12, // "fully tested"
  1899, 10, // "functional"
  1909, 14, // "functionalized"
  1923, 8, // "fungible"
  1931, 12, // "future-proof"
  1943, 6, // "global"
  1949, 10, // "go forward"
  1959, 13, // "goal-oriented"
  1972, 8, // "granular"
  1980, 17, // "high standards in"
  1997, 11, // "high-payoff"
  2008, 12, // "high-quality"
  2020, 16, // "highly efficient"
  2036, 8, // "holistic"
  2044, 9, // "impactful"
  2053, 11, // "inexpensive"
  2064, 10, // "innovative"
  2074, 14, // "installed base"
  2088, 10, // "integrated"
  2098, 11, // "interactive"
  2109, 14, // "interdependent"
  2123, 13, // "intermandated"
  2136, 13, // "interoperable"
  2149, 9, // "intuitive"
  2158, 12, // "just in time"
  2170, 12, // "leading-edge"
  2182, 9, // "leveraged"
  2191, 21, // "long-term high-impact"
  2212, 19, // "low-risk high-yield"
  2231, 8, // "magnetic"
  2239, 12, // "maintainable"
  2251, 18, // "market positioning"
  2269, 13, // "market-driven"
  2282, 16, // "mission-critical"
  2298, 17, // "multidisciplinary"
  2315, 15, // "multifunctional"
  2330, 16, // "multimedia based"
  2346, 15, // "next-generation"
  2361, 10, // "one-to-one"
  2371, 11, // "open-source"
  2382, 7, // "optimal"
  2389, 10, // "orthogonal"
  2399, 14, // "out-of-the-box"
  2413, 8, // "pandemic"
  2421, 8, // "parallel"
  2429, 17, // "performance based"
  2446, 13, // "plug-and-play"
  2459, 7, // "premier"
  2466, 7, // "premium"
  2473, 18, // "principle-centered"
  2491, 9, // "proactive"
  2500, 15, // "process-centric"
  2515, 12, // "professional"
  2527, 11, // "progressive"
  2538, 11, // "prospective"
  2549, 7, // "quality"
  2556, 9, // "real-time"
  2565, 8, // "reliable"
  2573, 16, // "resource sucking"
  2589, 19, // "resource maximizing"
  2608, 17, // "resource-leveling"
  2625, 13, // "revolutionary"
  2638, 6, // "robust"
  2644, 8, // "scalable"
  2652, 8, // "seamless"
  2660, 11, // "stand-alone"
  2671, 12, // "standardized"
  2683, 19, // "standards compliant"
  2702, 16, // "state of the art"
  2718, 6, // "sticky"
  2724, 9, // "strategic"
  2733, 8, // "superior"
  2741, 11, // "sustainable"
  2752, 11, // "synergistic"
  2763, 8, // "tactical"
  2771, 13, // "team building"
  2784, 11, // "team driven"
  2795, 17, // "technically sound"
  2812, 6, // "timely"
  2818, 8, // "top-line"
  2826, 11, // "transparent"
  2837, 7, // "turnkey"
  2844, 10, // "ubiquitous"
  2854, 6, // "unique"
  2860, 12, // "user-centric"
  2872, 13, // "user friendly"
  2885, 11, // "value-added"
  2896, 8, // "vertical"
  2904, 5, // "viral"
  2909, 7, // "virtual"
  2916, 9, // "visionary"
  2925, 11, // "web-enabled"
  2936, 8, // "wireless"
  2944, 11, // "world-class"
  2955, 9, // "worldwide"
};
#define NUM_ADJECTIVES 154
static const uint16_t NOUNS[] PROGMEM = {
  2964, 12, // "action items"
  2976, 10, // "alignments"
  2986, 12, // "applications"
  2998, 13, // "architectures"
  3011, 9, // "bandwidth"
  3020, 8, // "benefits"
  3028, 14, // "best practices"
  3042, 20, // "catalysts for change"
  3062, 8, // "channels"
  3070, 6, // "clouds"
  3076, 30, // "collaboration and idea-sharing"
  3106, 11, // "communities"
  3117, 7, // "content"
  3124, 11, // "convergence"
  3135, 17, // "core competencies"
  3152, 16, // "customer service"
  3168, 4, // "data"
  3172, 12, // "deliverables"
  3184, 10, // "e-business"
  3194, 10, // "e-commerce"
  3204, 9, // "e-markets"
  3213, 9, // "e-tailers"
  3222, 10, // "e-services"
  3232, 11, // "experiences"
  3243, 9, // "expertise"
  3252, 15, // "functionalities"
  3267, 11, // "fungibility"
  3278, 17, // "growth strategies"
  3295, 13, // "human capital"
  3308, 5, // "ideas"
  3313, 11, // "imperatives"
  3324, 13, // "infomediaries"
  3337, 11, // "information"
  3348, 15, // "infrastructures"
  3363, 11, // "initiatives"
  3374, 10, // "innovation"
  3384, 20, // "intellectual capital"
  3404, 10, // "interfaces"
  3414, 29, // "internal or "organic" sources"
  3443, 10, // "leadership"
  3453, 17, // "leadership skills"
  3470, 21, // "manufactured products"
  3491, 7, // "markets"
  3498, 9, // "materials"
  3507, 13, // "meta-services"
  3520, 13, // "methodologies"
  3533, 22, // "methods of empowerment"
  3555, 7, // "metrics"
  3562, 9, // "mindshare"
  3571, 6, // "models"
  3577, 8, // "networks"
  3585, 6, // "niches"
  3591, 13, // "niche markets"
  3604, 5, // "nosql"
  3609, 13, // "opportunities"
  3622, 26, // ""outside the box" thinking"
  3648, 11, // "outsourcing"
  3659, 9, // "paradigms"
  3668, 12, // "partnerships"
  3680, 9, // "platforms"
  3689, 7, // "portals"
  3696, 14, // "potentialities"
  3710, 20, // "process improvements"
  3730, 9, // "processes"
  3739, 8, // "products"
  3747, 15, // "quality vectors"
  3762, 13, // "relationships"
  3775, 9, // "resources"
  3784, 7, // "results"
  3791, 3, // "ROI"
  3794, 9, // "scenarios"
  3803, 7, // "schemas"
  3810, 8, // "services"
  3818, 9, // "solutions"
  3827, 7, // "sources"
  3834, 21, // "strategic theme areas"
  3855, 7, // "storage"
  3862, 13, // "supply chains"
  3875, 7, // "synergy"
  3882, 7, // "systems"
  3889, 12, // "technologies"
  3901, 10, // "technology"
  3911, 18, // "testing procedures"
  3929, 13, // "total linkage"
  3942, 5, // "users"
  3947, 5, // "value"
  3952, 7, // "vortals"
  3959, 13, // "web-readiness"
  3972, 12, // "web services"
};
#define NUM_NOUNS 89
static const uint16_t JOINS[] PROGMEM = {
  3984, 3, // "for"
  3987, 3, // "and"
  3990, 5, // "while"
  3995, 2, // "by"
  3997, 2, // "to"
};
#define NUM_JOINS 5
static const uint16_t DELIMS[] PROGMEM = {
  3999, 1, // " "
  4000, 2, // ", "
  4002, 5, // " and "
  4007, 6, // ", and "
};
#define NUM_DELIMS 4
#define TOTAL_WORDS 383
#define MAX_WORD_LEN 30

static uint16_t
jim_unique_random_int(
  jim_sentence_t * const sentence,
  const uint16_t max,
  const uint16_t * const words,
  const uint8_t num_words,
  uint16_t (*random_int)(jim_sentence_t * const, uint16_t, uint16_t)
) {
  uint16_t r;
  
retry:
  r = random_int(sentence, 0, max);

  for (uint8_t i = 0; i < num_words; i++) {
    if (r == words[i]) {
      goto retry;
    }
  }

  // return word
  return r;
}

void
jim_sentence_init(
  jim_sentence_t * const r,
  uint16_t (*random_int)(jim_sentence_t * const, uint16_t, uint16_t),
  void * const user_data
) {
  r->user_data = user_data;
  r->num_phrases = random_int(r, 1, 4);
  for (uint8_t phrase_i = 0; phrase_i < r->num_phrases; phrase_i++) {
    // adverb
    r->phrases[phrase_i].has_adverb = (random_int(r, 0, 2) == 1);
    r->phrases[phrase_i].adverb = random_int(r, 0, NUM_ADVERBS);

    // verb
    r->phrases[phrase_i].verb = random_int(r, 0, NUM_VERBS);

    // adjectives
    r->phrases[phrase_i].num_adjectives = random_int(r, 0, 5);
    for (uint8_t adj_i = 0; adj_i < r->phrases[phrase_i].num_adjectives; adj_i++) {
      // get unique adjective
      r->phrases[phrase_i].adjectives[adj_i] = jim_unique_random_int(
        r,
        NUM_ADJECTIVES,
        r->phrases[phrase_i].adjectives,
        adj_i,
        random_int
      );
    }

    // noun
    r->phrases[phrase_i].noun = random_int(r, 0, NUM_NOUNS);

    if (phrase_i < (r->num_phrases - 1)) {
      // add join word
      r->joins[phrase_i] = jim_unique_random_int(
        r,
        NUM_JOINS,
        r->joins,
        phrase_i,
        random_int
      );
    }
  }
}

static void
send_word(
  const jim_sentence_t * const sentence,
  const uint16_t * const words,
  const uint16_t word_id,
  char * const buf,
  void (*text_cb)(const jim_sentence_t * const, const char * const)
) {
  const uint16_t ofs = GET_WORD_OFFSET(words, word_id),
                 len = GET_WORD_LENGTH(words, word_id);

  // populate/terminate buffer
  memcpy_P(buf, TEXT + ofs, len);
  buf[len] = '\0';

  // send word
  text_cb(sentence, buf);
}

static void
send_delim(
  const jim_sentence_t * const sentence,
  const delim_t delim,
  char * const buf,
  void (*text_cb)(const jim_sentence_t * const, const char * const)
) {
  send_word(sentence, DELIMS, delim, buf, text_cb);
}

static void
jim_phrase_emit(
  const jim_sentence_t * const sentence,
  const jim_phrase_t * const phrase,
  char * const buf,
  void (*text_cb)(const jim_sentence_t * const, const char * const)
) {
  // adverb
  if (phrase->has_adverb) {
    // send adverb, send space
    send_word(sentence, ADVERBS, phrase->adverb, buf, text_cb);
    send_delim(sentence, DELIM_SPACE, buf, text_cb);
  }

  // verb
  {
    // send verb, send space
    send_word(sentence, VERBS, phrase->verb, buf, text_cb);
    send_delim(sentence, DELIM_SPACE, buf, text_cb);
  }

  // adjectives
  for (uint8_t i = 0; i < phrase->num_adjectives; i++) {
    // send adjective
    send_word(sentence, ADJECTIVES, phrase->adjectives[i], buf, text_cb);

    if (phrase->num_adjectives == 1) {
      // send tail space
      send_delim(sentence, DELIM_SPACE, buf, text_cb);
    } else if (phrase->num_adjectives == 2) {
      if (i == 0) {
        // send delimiting " and "
        send_delim(sentence, DELIM_AND, buf, text_cb);
      } else {
        // send tail " "
        send_delim(sentence, DELIM_SPACE, buf, text_cb);
      }
    } else if (phrase->num_adjectives > 2) { 
      if (i == phrase->num_adjectives - 2) {
        // send ", and " before final adjective
        send_delim(sentence, DELIM_COMMA_AND, buf, text_cb);
      } else if (i < phrase->num_adjectives - 2) {
        // send delimiting ", "
        send_delim(sentence, DELIM_COMMA, buf, text_cb);
      } else {
        // send tail " "
        send_delim(sentence, DELIM_SPACE, buf, text_cb);
      }
    }
  }

  // send noun
  send_word(sentence, NOUNS, phrase->noun, buf, text_cb);
}

void
jim_sentence_emit(
  const jim_sentence_t * const sentence,
  char * const buf,
  void (*text_cb)(const jim_sentence_t * const, const char * const)
) {
  for (uint8_t i = 0; i < sentence->num_phrases; i++) {
    jim_phrase_emit(sentence, sentence->phrases + i, buf, text_cb);

    // send join word
    if (sentence->num_phrases > 1 && i < (sentence->num_phrases - 1)) {
      // send space, join, space
      send_delim(sentence, DELIM_SPACE, buf, text_cb);
      send_word(sentence, JOINS, sentence->joins[i], buf, text_cb);
      send_delim(sentence, DELIM_SPACE, buf, text_cb);
    }
  }
}

void *
jim_sentence_get_data(
  const jim_sentence_t * const sentence
) {
  return sentence->user_data;
}
