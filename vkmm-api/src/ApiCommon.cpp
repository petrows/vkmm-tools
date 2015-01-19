#include "ApiCommon.h"

#include <QDateTime>

namespace VKMM {

QString formatDateTime(time_t tms)
{
	QDateTime tt;
	tt.setTime_t(tms);
	return tt.toString("yyyy-MM-dd hh:mm:ss");
}

}
