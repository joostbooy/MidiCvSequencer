#include "lookupTables.h"

const uint32_t lut_bpm_inc[300] = {
	1717986,3435973,5153960,6871947,8589934,10307921,12025908,
	13743895,15461882,17179869,18897856,20615843,22333829,24051816,25769803,
	27487790,29205777,30923764,32641751,34359738,36077725,37795712,39513699,
	41231686,42949672,44667659,46385646,48103633,49821620,51539607,53257594,
	54975581,56693568,58411555,60129542,61847529,63565515,65283502,67001489,
	68719476,70437463,72155450,73873437,75591424,77309411,79027398,80745385,
	82463372,84181359,85899345,87617332,89335319,91053306,92771293,94489280,
	96207267,97925254,99643241,101361228,103079215,104797202,106515188,108233175,
	109951162,111669149,113387136,115105123,116823110,118541097,120259084,121977071,
	123695058,125413045,127131031,128849018,130567005,132284992,134002979,135720966,
	137438953,139156940,140874927,142592914,144310901,146028888,147746874,149464861,
	151182848,152900835,154618822,156336809,158054796,159772783,161490770,163208757,
	164926744,166644731,168362718,170080704,171798691,173516678,175234665,176952652,
	178670639,180388626,182106613,183824600,185542587,187260574,188978561,190696547,
	192414534,194132521,195850508,197568495,199286482,201004469,202722456,204440443,
	206158430,207876417,209594404,211312390,213030377,214748364,216466351,218184338,
	219902325,221620312,223338299,225056286,226774273,228492260,230210247,231928233,
	233646220,235364207,237082194,238800181,240518168,242236155,243954142,245672129,
	247390116,249108103,250826090,252544077,254262063,255980050,257698037,259416024,
	261134011,262851998,264569985,266287972,268005959,269723946,271441933,273159920,
	274877906,276595893,278313880,280031867,281749854,283467841,285185828,286903815,
	288621802,290339789,292057776,293775763,295493749,297211736,298929723,300647710,
	302365697,304083684,305801671,307519658,309237645,310955632,312673619,314391606,
	316109592,317827579,319545566,321263553,322981540,324699527,326417514,328135501,
	329853488,331571475,333289462,335007449,336725436,338443422,340161409,341879396,
	343597383,345315370,347033357,348751344,350469331,352187318,353905305,355623292,
	357341279,359059265,360777252,362495239,364213226,365931213,367649200,369367187,
	371085174,372803161,374521148,376239135,377957122,379675108,381393095,383111082,
	384829069,386547056,388265043,389983030,391701017,393419004,395136991,396854978,
	398572965,400290951,402008938,403726925,405444912,407162899,408880886,410598873,
	412316860,414034847,415752834,417470821,419188808,420906795,422624781,424342768,
	426060755,427778742,429496729,431214716,432932703,434650690,436368677,438086664,
	439804651,441522638,443240624,444958611,446676598,448394585,450112572,451830559,
	453548546,455266533,456984520,458702507,460420494,462138481,463856467,465574454,
	467292441,469010428,470728415,472446402,474164389,475882376,477600363,479318350,
	481036337,482754324,484472310,486190297,487908284,489626271,491344258,493062245,
	494780232,496498219,498216206,499934193,501652180,503370167,505088154,506806140,
	508524127,510242114,511960101,513678088,515396075,
};

const uint32_t lut_bpm_fractional_inc[10] = {
	0,171798,343597,515396,687194,858993,1030792,
	1202590,1374389,1546188,
};

const uint8_t lut_scale_major[70] = {
	0,2,4,5,7,9,11,
	12,14,16,17,19,21,23,24,
	26,28,29,31,33,35,36,38,
	40,41,43,45,47,48,50,52,
	53,55,57,59,60,62,64,65,
	67,69,71,72,74,76,77,79,
	81,83,84,86,88,89,91,93,
	95,96,98,100,101,103,105,107,
	108,110,112,113,115,117,119,
};

const uint8_t lut_scale_major_map[128] = {
	0,0,2,2,4,5,5,
	7,7,9,9,11,12,12,14,
	14,16,17,17,19,19,21,21,
	23,24,24,26,26,28,29,29,
	31,31,33,33,35,36,36,38,
	38,40,41,41,43,43,45,45,
	47,48,48,50,50,52,53,53,
	55,55,57,57,59,60,60,62,
	62,64,65,65,67,67,69,69,
	71,72,72,74,74,76,77,77,
	79,79,81,81,83,84,84,86,
	86,88,89,89,91,91,93,93,
	95,96,96,98,98,100,101,101,
	103,103,105,105,107,108,108,110,
	110,112,113,113,115,115,117,117,
	119,120,120,122,122,124,125,125,
	127,
};

const uint8_t lut_scale_minor[70] = {
	0,2,3,5,7,8,10,
	12,14,15,17,19,20,22,24,
	26,27,29,31,32,34,36,38,
	39,41,43,44,46,48,50,51,
	53,55,56,58,60,62,63,65,
	67,68,70,72,74,75,77,79,
	80,82,84,86,87,89,91,92,
	94,96,98,99,101,103,104,106,
	108,110,111,113,115,116,118,
};

const uint8_t lut_scale_minor_map[128] = {
	0,0,2,3,3,5,5,
	7,8,8,10,10,12,12,14,
	15,15,17,17,19,20,20,22,
	22,24,24,26,27,27,29,29,
	31,32,32,34,34,36,36,38,
	39,39,41,41,43,44,44,46,
	46,48,48,50,51,51,53,53,
	55,56,56,58,58,60,60,62,
	63,63,65,65,67,68,68,70,
	70,72,72,74,75,75,77,77,
	79,80,80,82,82,84,84,86,
	87,87,89,89,91,92,92,94,
	94,96,96,98,99,99,101,101,
	103,104,104,106,106,108,108,110,
	111,111,113,113,115,116,116,118,
	118,120,120,122,123,123,125,125,
	127,
};

const uint8_t lut_scale_mixolydian[70] = {
	0,2,4,5,7,9,10,
	12,14,16,17,19,21,22,24,
	26,28,29,31,33,34,36,38,
	40,41,43,45,46,48,50,52,
	53,55,57,58,60,62,64,65,
	67,69,70,72,74,76,77,79,
	81,82,84,86,88,89,91,93,
	94,96,98,100,101,103,105,106,
	108,110,112,113,115,117,118,
};

const uint8_t lut_scale_mixolydian_map[128] = {
	0,0,2,2,4,5,5,
	7,7,9,10,10,12,12,14,
	14,16,17,17,19,19,21,22,
	22,24,24,26,26,28,29,29,
	31,31,33,34,34,36,36,38,
	38,40,41,41,43,43,45,46,
	46,48,48,50,50,52,53,53,
	55,55,57,58,58,60,60,62,
	62,64,65,65,67,67,69,70,
	70,72,72,74,74,76,77,77,
	79,79,81,82,82,84,84,86,
	86,88,89,89,91,91,93,94,
	94,96,96,98,98,100,101,101,
	103,103,105,106,106,108,108,110,
	110,112,113,113,115,115,117,118,
	118,120,120,122,122,124,125,125,
	127,
};

const uint8_t lut_scale_dorian[70] = {
	0,2,3,5,7,9,10,
	12,14,15,17,19,21,22,24,
	26,27,29,31,33,34,36,38,
	39,41,43,45,46,48,50,51,
	53,55,57,58,60,62,63,65,
	67,69,70,72,74,75,77,79,
	81,82,84,86,87,89,91,93,
	94,96,98,99,101,103,105,106,
	108,110,111,113,115,117,118,
};

const uint8_t lut_scale_dorian_map[128] = {
	0,0,2,3,3,5,5,
	7,7,9,10,10,12,12,14,
	15,15,17,17,19,19,21,22,
	22,24,24,26,27,27,29,29,
	31,31,33,34,34,36,36,38,
	39,39,41,41,43,43,45,46,
	46,48,48,50,51,51,53,53,
	55,55,57,58,58,60,60,62,
	63,63,65,65,67,67,69,70,
	70,72,72,74,75,75,77,77,
	79,79,81,82,82,84,84,86,
	87,87,89,89,91,91,93,94,
	94,96,96,98,99,99,101,101,
	103,103,105,106,106,108,108,110,
	111,111,113,113,115,115,117,118,
	118,120,120,122,123,123,125,125,
	127,
};

const uint8_t lut_scale_phrygian[70] = {
	0,1,3,5,7,8,10,
	12,13,15,17,19,20,22,24,
	25,27,29,31,32,34,36,37,
	39,41,43,44,46,48,49,51,
	53,55,56,58,60,61,63,65,
	67,68,70,72,73,75,77,79,
	80,82,84,85,87,89,91,92,
	94,96,97,99,101,103,104,106,
	108,109,111,113,115,116,118,
};

const uint8_t lut_scale_phrygian_map[128] = {
	0,1,1,3,3,5,5,
	7,8,8,10,10,12,13,13,
	15,15,17,17,19,20,20,22,
	22,24,25,25,27,27,29,29,
	31,32,32,34,34,36,37,37,
	39,39,41,41,43,44,44,46,
	46,48,49,49,51,51,53,53,
	55,56,56,58,58,60,61,61,
	63,63,65,65,67,68,68,70,
	70,72,73,73,75,75,77,77,
	79,80,80,82,82,84,85,85,
	87,87,89,89,91,92,92,94,
	94,96,97,97,99,99,101,101,
	103,104,104,106,106,108,109,109,
	111,111,113,113,115,116,116,118,
	118,120,121,121,123,123,125,125,
	127,
};

const uint8_t lut_scale_lydian[70] = {
	0,2,4,6,7,9,11,
	12,14,16,18,19,21,23,24,
	26,28,30,31,33,35,36,38,
	40,42,43,45,47,48,50,52,
	54,55,57,59,60,62,64,66,
	67,69,71,72,74,76,78,79,
	81,83,84,86,88,90,91,93,
	95,96,98,100,102,103,105,107,
	108,110,112,114,115,117,119,
};

const uint8_t lut_scale_lydian_map[128] = {
	0,0,2,2,4,4,6,
	7,7,9,9,11,11,12,12,
	14,14,16,16,18,19,19,21,
	21,23,23,24,24,26,26,28,
	28,30,31,31,33,33,35,35,
	36,36,38,38,40,40,42,43,
	43,45,45,47,47,48,48,50,
	50,52,52,54,55,55,57,57,
	59,59,60,60,62,62,64,64,
	66,67,67,69,69,71,71,72,
	72,74,74,76,76,78,79,79,
	81,81,83,83,84,84,86,86,
	88,88,90,91,91,93,93,95,
	95,96,96,98,98,100,100,102,
	103,103,105,105,107,107,108,108,
	110,110,112,112,114,115,115,117,
	117,
};

const uint8_t lut_scale_aeolyian[70] = {
	0,2,3,5,7,8,10,
	12,14,15,17,19,20,22,24,
	26,27,29,31,32,34,36,38,
	39,41,43,44,46,48,50,51,
	53,55,56,58,60,62,63,65,
	67,68,70,72,74,75,77,79,
	80,82,84,86,87,89,91,92,
	94,96,98,99,101,103,104,106,
	108,110,111,113,115,116,118,
};

const uint8_t lut_scale_aeolyian_map[128] = {
	0,0,2,3,3,5,5,
	7,8,8,10,10,12,12,14,
	15,15,17,17,19,20,20,22,
	22,24,24,26,27,27,29,29,
	31,32,32,34,34,36,36,38,
	39,39,41,41,43,44,44,46,
	46,48,48,50,51,51,53,53,
	55,56,56,58,58,60,60,62,
	63,63,65,65,67,68,68,70,
	70,72,72,74,75,75,77,77,
	79,80,80,82,82,84,84,86,
	87,87,89,89,91,92,92,94,
	94,96,96,98,99,99,101,101,
	103,104,104,106,106,108,108,110,
	111,111,113,113,115,116,116,118,
	118,120,120,122,123,123,125,125,
	127,
};

const uint8_t lut_scale_locrian[70] = {
	0,1,3,5,6,8,10,
	12,13,15,17,18,20,22,24,
	25,27,29,30,32,34,36,37,
	39,41,42,44,46,48,49,51,
	53,54,56,58,60,61,63,65,
	66,68,70,72,73,75,77,78,
	80,82,84,85,87,89,90,92,
	94,96,97,99,101,102,104,106,
	108,109,111,113,114,116,118,
};

const uint8_t lut_scale_locrian_map[128] = {
	0,1,1,3,3,5,6,
	6,8,8,10,10,12,13,13,
	15,15,17,18,18,20,20,22,
	22,24,25,25,27,27,29,30,
	30,32,32,34,34,36,37,37,
	39,39,41,42,42,44,44,46,
	46,48,49,49,51,51,53,54,
	54,56,56,58,58,60,61,61,
	63,63,65,66,66,68,68,70,
	70,72,73,73,75,75,77,78,
	78,80,80,82,82,84,85,85,
	87,87,89,90,90,92,92,94,
	94,96,97,97,99,99,101,102,
	102,104,104,106,106,108,109,109,
	111,111,113,114,114,116,116,118,
	118,120,121,121,123,123,125,126,
	126,
};