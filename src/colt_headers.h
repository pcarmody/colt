#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <string.h>
#include <limits.h>
#include <bitset>

#pragma GCC diagnostic ignored "-Wwrite-strings"

#include "coltdatatype.h"

#include "coltbase.h"
#include "coltcsv.h"
#include "coltsequentialcsv.h"
#include "coltoperator.h"
#include "coltreduce.h"
#include "coltthru.h"
#include "coltsort.h"
#include "coltkeyspace.h"
#include "coltcthru.h"
#include "coltcexpression.h"
#include "coltif.h"
#include "coltaggregate.h"
#include "coltaggregagerow.h"
#include "colteach.h"
#include "coltonchange.h"

#include "coltlink.h"
#include "avl.h"
#include "AssocArray.h"
#include "coltpartition.h"
#include "coltshare.h"
#include "coltcthru.h"

#include <thread>
#include "concurrentqueue.h"
#include "coltthread.h"
#include "coltsync.h"

#include "coltout.h"
#include "coltoutcbf.h"
#include "colt_html.h"
#include "colt_out_vertical.h"
#include "colt_skip_limit.h"
#include "colt_select.h"
#include "coltadd.h"
#include "colt_to_json.h"
#include "coltcounter.h"

#include "colt_cross.h"
#include "coltexpand.h"
#include "colt_range.h"
#include "colt_range_list.h"
#include "coltbitmap.h"
#include "coltisect.h"
#include "coltunion.h"
#include "coltcomplement.h"

#include "coltparser.h"
#include "colttrace.h"

#define COLT_TRACE(MSG) colt_trace _trace(MSG);
#define TRACE(MSG) _trace << MSG << "\n";
using namespace std;
