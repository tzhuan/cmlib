/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/**
 * @file gaussian.h
 * @author Tz-Huan Huang <tzhuan@gmail.com>
 * @brief The groundtruth for Gaussian filter testing
 *
 * This groundtruth is generated using fspecial and imfilter in Matlab.
 */

#ifndef CMLIB_DIP_TEST_GAUSSIAN_H
#define CMLIB_DIP_TEST_GAUSSIAN_H

/**
 * gaussian_kernel = fspecial('gaussian', [7 7], 1.0)
 */
double gaussian_kernel[] = { // sigma = 1.0, window size = 7x7
	1.9651916124031903e-05, 0.000239409349497269827, 0.00107295826497866005, 0.00176900911404382125, 0.00107295826497866005, 0.000239409349497269827, 1.9651916124031903e-05,
	0.000239409349497269827, 0.00291660295438643654, 0.0130713075831894038, 0.0215509428482682543, 0.0130713075831894038, 0.00291660295438643654, 0.000239409349497269827,
	0.00107295826497866005, 0.0130713075831894038, 0.0585815363306070175, 0.0965846250185641159, 0.0585815363306070175, 0.0130713075831894038, 0.00107295826497866005,
	0.00176900911404382125, 0.0215509428482682543, 0.0965846250185641159, 0.159241125690702423, 0.0965846250185641159, 0.0215509428482682543, 0.00176900911404382125,
	0.00107295826497866005, 0.0130713075831894038, 0.0585815363306070175, 0.0965846250185641159, 0.0585815363306070175, 0.0130713075831894038, 0.00107295826497866005,
	0.000239409349497269827, 0.00291660295438643654, 0.0130713075831894038, 0.0215509428482682543, 0.0130713075831894038, 0.00291660295438643654, 0.000239409349497269827,
	1.9651916124031903e-05, 0.000239409349497269827, 0.00107295826497866005, 0.00176900911404382125, 0.00107295826497866005, 0.000239409349497269827, 1.9651916124031903e-05,
};

/**
 * gaussian_source = rand([10 10])
 */
double gaussian_source[] = {
	0.814723686393178936, 0.157613081677548283, 0.655740699156586837, 0.706046088019608775, 0.438744359656398242, 0.276025076998578367, 0.751267059305652851, 0.840717255983662537, 0.351659507062996757, 0.0758542895630636149,
	0.905791937075619225, 0.970592781760615697, 0.0357116785741895537, 0.031832846377420676, 0.381558457093008396, 0.679702676853674803, 0.255095115459269106, 0.254282178971531048, 0.830828627896290861, 0.0539501186666071497,
	0.126986816293506055, 0.95716694824294557, 0.849129305868777107, 0.276922984960889962, 0.76551678814900237, 0.655098003973840659, 0.505957051665142377, 0.814284826068816359, 0.585264091152724264, 0.530797553008972689,
	0.913375856139019393, 0.485375648722841224, 0.933993247757550549, 0.0461713906311539413, 0.795199901137063181, 0.162611735194630569, 0.699076722656685967, 0.243524968724989299, 0.549723608291139532, 0.779167230102011166,
	0.63235924622540951, 0.800280468888800112, 0.678735154857773471, 0.0971317812358475363, 0.186872604554378596, 0.118997681558376645, 0.890903252535798496, 0.929263623187227772, 0.917193663829810046, 0.934010684229182986,
	0.0975404049994095246, 0.141886338627215336, 0.757740130578333448, 0.82345782832729264, 0.489764395788231055, 0.498364051982142953, 0.959291425205444304, 0.349983765984808737, 0.285839018820373547, 0.129906208473730134,
	0.278498218867048397, 0.421761282626274991, 0.743132468124916179, 0.694828622975817045, 0.445586200710899494, 0.959743958516081075, 0.547215529963803071, 0.196595250431208179, 0.757200229110721268, 0.568823660872192716,
	0.546881519204983846, 0.91573552518906709, 0.392227019534168164, 0.317099480060860528, 0.64631301011126463, 0.340385726666133204, 0.13862444282867914, 0.251083857976031055, 0.753729094278495326, 0.469390641058205826,
	0.957506835434297598, 0.792207329559554418, 0.655477890177556644, 0.950222048838354927, 0.709364830858072559, 0.585267750979777346, 0.149294005559057474, 0.616044676146639159, 0.380445846975356661, 0.0119020695012413968,
	0.964888535199276531, 0.959492426392902997, 0.171186687811561766, 0.0344460805029087602, 0.754686681982360885, 0.22381193949113698, 0.257508254123736458, 0.473288848902729264, 0.567821640725221122, 0.33712264439888151,
};

/**
 * gaussian_default_result = imfilter(gaussian_source, gaussian_kernel)
 */
double gaussian_default_result[] = {
	0.32533874648358424, 0.370084774356587487, 0.334893707735089696, 0.314644766229388917, 0.315551230091309198, 0.33787559525873273, 0.377456270377913239, 0.384402171459555575, 0.297637715630096333, 0.146596404141755704,
	0.446700654395538632, 0.549772192408900739, 0.463198731118166396, 0.392961196530775381, 0.431212928906257509, 0.474538707687905958, 0.487211478119478736, 0.499987238363861108, 0.440318937533033028, 0.258960682456151225,
	0.453527791984033646, 0.621173048941092043, 0.558834248262880973, 0.452792402876292965, 0.480256960548959999, 0.518435531527206006, 0.530559007280802075, 0.5525471815457067, 0.529684223494495288, 0.372911670334389045,
	0.450805853550569902, 0.621978967058742116, 0.586486267063535904, 0.460826320882829321, 0.441204867813299761, 0.48493301772159747, 0.558225392756936389, 0.60218958492559338, 0.596114914058921808, 0.460654434102460097,
	0.386565454367157835, 0.552561137707760386, 0.568341520026003688, 0.469231127371646961, 0.418121710680428804, 0.480548237087960906, 0.599959115745101523, 0.635671478190582229, 0.600539070865982794, 0.452411395414075879,
	0.283941645104417884, 0.463747636312371758, 0.570804147015198549, 0.552945113002196287, 0.509100001251210132, 0.54925805059354893, 0.597891246610391613, 0.564724113893440904, 0.506538040015173996, 0.364972367964976896,
	0.307142348557088107, 0.486610924084577956, 0.582599919398911648, 0.589968762550290959, 0.57017106191586675, 0.559869597914158246, 0.509910271907992096, 0.467082619185561099, 0.461884275269579925, 0.342203436507953929,
	0.437437011027034361, 0.594250880012203497, 0.598221674797045688, 0.579813070335587932, 0.563080796013439411, 0.491530137380343068, 0.404353609298854666, 0.412138801519458087, 0.438290010923819195, 0.314097293745853046,
	0.522776880032352897, 0.638410647370213802, 0.568651667265244298, 0.535455492865799521, 0.523239128695798139, 0.426934318942053415, 0.349097228669337623, 0.381364088541427448, 0.378001062778280328, 0.240889574441050247,
	0.424661311366754668, 0.483890680228895032, 0.379347037441075119, 0.340586664232255143, 0.354624881591050212, 0.296599329878539653, 0.255077487803618497, 0.289319195397990114, 0.279207605158577565, 0.173441394502375834,
};

/**
 * gaussian_default_0_1_result = imfilter(gaussian_source, gaussian_kernel, 0.1)
 */
double gaussian_default_0_1_result[] = { // default value = 0.1
	0.376405204358694068, 0.404220189511963202, 0.365251296616931231, 0.344692252246766195, 0.345598716108686421, 0.367923081276109953, 0.407503756395290462, 0.414759760341397221, 0.331773130785472048, 0.197662862016865365,
	0.480836069550914402, 0.561118411211481871, 0.469459992888894395, 0.39880505961054119, 0.437056791986023319, 0.480382570767671768, 0.493055341199244546, 0.506248500134589108, 0.451665156335614104, 0.29309609761152694,
	0.483885380865875292, 0.627434310711820098, 0.55971889270631725, 0.45323570769381738, 0.480700265366484303, 0.518878836344730421, 0.53100231209832649, 0.553431825989143089, 0.535945485265223343, 0.40326925921623058,
	0.48085333956794718, 0.627822830138508037, 0.586929571881060208, 0.460826320882829321, 0.441204867813299761, 0.48493301772159747, 0.558225392756936389, 0.602632889743117683, 0.601958777138687617, 0.49070192011983732,
	0.416612940384535058, 0.558405000787525974, 0.568784824843527992, 0.469231127371646961, 0.418121710680428804, 0.480548237087960906, 0.599959115745101523, 0.636114783008106532, 0.606382933945748714, 0.482458881431453157,
	0.313989131121795106, 0.469591499392137623, 0.571247451832722963, 0.552945113002196287, 0.509100001251210132, 0.54925805059354893, 0.597891246610391613, 0.565167418710965208, 0.512381903094939806, 0.395019853982354119,
	0.337189834574465386, 0.492454787164343766, 0.583043224216436062, 0.589968762550290959, 0.57017106191586675, 0.559869597914158246, 0.509910271907992096, 0.467525924003085458, 0.467728138349345679, 0.372250922525331152,
	0.467794599908875952, 0.600512141782931774, 0.599106319240482077, 0.580256375153112347, 0.563524100830963826, 0.491973442197867428, 0.404796914116379025, 0.41302344596289442, 0.44455127269454725, 0.344454882627694636,
	0.556912295187728668, 0.649756866172794934, 0.574912929035972242, 0.54129935594556533, 0.529082991775563949, 0.432778182021819224, 0.354941091749103432, 0.387625350312155448, 0.389347281580861515, 0.275024989596425962,
	0.475727769241864384, 0.518026095384270691, 0.409704626322916654, 0.370634150249632366, 0.384672367608427435, 0.326646815895916875, 0.28512497382099572, 0.31967678427983176, 0.31334302031395328, 0.224507852377485551,
};

/**
 * gaussian_circular_result = imfilter(gaussian_source, gaussian_kernel, 'circular')
 */
double gaussian_circular_result[] = {
	0.582473940404203194, 0.585392012291979125, 0.477292350718192926, 0.433580175609378282, 0.450753927824658673, 0.456473959274090735, 0.484409961166218594, 0.514062805997908101, 0.471009699311216501, 0.467498834717535006,
	0.575768965570396252, 0.604297530492829749, 0.489420798664979284, 0.413046701785301285, 0.455568996729270537, 0.496602549912117941, 0.507652110318511318, 0.527700676769090937, 0.506326964465607765, 0.486632374895358266,
	0.607057643132321911, 0.652335909752358223, 0.562703449309639669, 0.454154078358757318, 0.481999898719311537, 0.52005357922931561, 0.532086424676379011, 0.556962958290703636, 0.566121203538954565, 0.558722537321827972,
	0.652442202161922613, 0.661481964791608146, 0.589492985180394613, 0.460826320882829321, 0.441204867813299761, 0.48493301772159747, 0.558225392756936389, 0.604881792821942321, 0.631934745893907968, 0.647968838581089668,
	0.582482163784712403, 0.59077676414896152, 0.571242735546634739, 0.469231127371646961, 0.418121710680428804, 0.480548237087960906, 0.599959115745101523, 0.638000423143511686, 0.631367356343173869, 0.612892265333838671,
	0.435482629026654022, 0.49259596590760113, 0.572956012328636133, 0.552945113002196287, 0.509100001251210132, 0.54925805059354893, 0.597891246610391613, 0.566244887450871315, 0.526996870389091487, 0.474753692116145654,
	0.446087638424381872, 0.512516842151545138, 0.584497591739194799, 0.589968762550290959, 0.57017106191586675, 0.559869597914158246, 0.509910271907992096, 0.468684263201365692, 0.483686828386368062, 0.460444277160252713,
	0.561217827772073741, 0.618378976615225451, 0.60219898035552899, 0.582371000838249997, 0.565267130073961233, 0.493685367795022534, 0.407085421406081116, 0.417478711164502858, 0.474672957728784761, 0.496980109383350688,
	0.635252089958122879, 0.68103621434968975, 0.600299265191460263, 0.567539401619750694, 0.551406251498180011, 0.455161431185123588, 0.384122848125008798, 0.420425831851727438, 0.449091290588687631, 0.492869453165061144,
	0.630706098056989295, 0.643583197598926016, 0.531636895355035, 0.494097172575916344, 0.495373193868253014, 0.44109554004365864, 0.428302799592308325, 0.468981001933667097, 0.453925067871824572, 0.484966515214403426,
};

/**
 * gaussian_replicate_result = imfilter(gaussian_source, gaussian_kernel, 'replicate')
 */
double gaussian_replicate_result[] = {
	0.68647004228340458, 0.554720139228825904, 0.50058513239786262, 0.488022942991329256, 0.463742360699162415, 0.483958500720833462, 0.562620254275913334, 0.572149215357508178, 0.424761432738744227, 0.225428335721933437,
	0.677246945438854109, 0.616859927618743753, 0.497788797001233019, 0.426681099934016983, 0.460034264455166708, 0.502950019613342292, 0.523223574724768214, 0.537387904520128301, 0.476725201891025019, 0.334359480229374506,
	0.627533192268743933, 0.656702734212554207, 0.56376735104704423, 0.455350333378954919, 0.48244329460948171, 0.520590761941885471, 0.53329081938802847, 0.557389378145079495, 0.558877497721568917, 0.514534875175895023,
	0.633285438530431177, 0.657468981393321883, 0.589178474959884957, 0.460826320882829321, 0.441204867813299761, 0.48493301772159747, 0.558225392756936389, 0.605196303042452088, 0.635750957475951006, 0.664451724371120767,
	0.544422843102942977, 0.583262440484564659, 0.570670464978933034, 0.469231127371646961, 0.418121710680428804, 0.480548237087960906, 0.599959115745101523, 0.638572693711213391, 0.638784326945403214, 0.649057651767406929,
	0.387020656195868062, 0.483795224548796088, 0.572324920572628959, 0.552945113002196287, 0.509100001251210132, 0.54925805059354893, 0.597891246610391613, 0.566875979206878267, 0.534904991513811101, 0.510827184697793313,
	0.415702811397215932, 0.507724586650118881, 0.584201563414716185, 0.589968762550290959, 0.57017106191586675, 0.559869597914158246, 0.509910271907992096, 0.468980291525844306, 0.486900329434757362, 0.470828889821662211,
	0.615046059393949074, 0.631192728586305818, 0.602586405492694932, 0.581174745818052285, 0.56482373418379106, 0.493148185082452728, 0.405881026694431657, 0.415600964435556497, 0.461193435039389588, 0.423208917858761802,
	0.805744693893846287, 0.72507040601745687, 0.595821706298695131, 0.553405771689824411, 0.546215400600337908, 0.448264223003243922, 0.369232399262004518, 0.407153153735072493, 0.418241252773900341, 0.333747704710224635,
	0.894649992192792065, 0.737909724343714823, 0.504516212483719917, 0.432881905344325746, 0.472762353218941334, 0.406271645140296622, 0.358606822523231339, 0.417380947992631368, 0.426285079085742535, 0.343282380074329208,
};

/**
 * gaussian_symmetric_result = imfilter(gaussian_source, gaussian_kernel, 'symmetric')
 */
double gaussian_symmetric_result[] = {
	0.676727189372286708, 0.562667453484772095, 0.489901128467458813, 0.468155274573050006, 0.45629954236851189, 0.482371805423851663, 0.550681582166603012, 0.560538998851441872, 0.428469149300486185, 0.255271472373455488,
	0.67780534976633644, 0.617636716203314506, 0.496858685086217222, 0.425045105284726443, 0.45938005170863927, 0.502765819930976132, 0.522237097575149911, 0.53642209806141139, 0.478558313160108773, 0.357570213211309551,
	0.639452322597375389, 0.657652474162535361, 0.56376735104704423, 0.455350333378954919, 0.48244329460948171, 0.520590761941885471, 0.53329081938802847, 0.557389378145079495, 0.55963171298625547, 0.524066150624207694,
	0.638120258029199472, 0.657798798893728387, 0.589178474959884957, 0.460826320882829321, 0.441204867813299761, 0.48493301772159747, 0.558225392756936389, 0.605196303042452088, 0.635617911791787726, 0.662290782713812698,
	0.547046324286920571, 0.583389423184951239, 0.570670464978933034, 0.469231127371646961, 0.418121710680428804, 0.480548237087960906, 0.599959115745101523, 0.638572693711213391, 0.63875469730718426, 0.648328104831630503,
	0.393722969255586586, 0.484206466686289139, 0.572324920572628959, 0.552945113002196287, 0.509100001251210132, 0.54925805059354893, 0.597891246610391613, 0.566875979206878267, 0.535386369610403534, 0.516513351887213035,
	0.425383189209387003, 0.508413477201366093, 0.584201563414716185, 0.589968762550290959, 0.57017106191586675, 0.559869597914158246, 0.509910271907992096, 0.468980291525844306, 0.487790193336547107, 0.481148726375247582,
	0.62271391253187558, 0.631834109384073694, 0.602586405492694932, 0.581174745818052285, 0.56482373418379106, 0.493148185082452728, 0.405881026694431657, 0.415600964435556497, 0.462390751102458419, 0.437745731856115705,
	0.805614327359028271, 0.725630811994996461, 0.597475861828281718, 0.555540998120325424, 0.547595196518811278, 0.448998161166265453, 0.36953786086837026, 0.407089927501347593, 0.418926158538394877, 0.349532993252264113,
	0.888217298022571766, 0.7427098204616569, 0.525183356368145726, 0.459522073612244564, 0.489827579324399742, 0.415197693893897657, 0.362031178591923908, 0.416349570317756545, 0.420063553827031033, 0.345429428624817014,
};

#endif // CMLIB_DIP_TEST_GAUSSIAN_H
