#include "tws_meta.h"

#include "twsapi/twsUtil.h"

#include <QtCore/QStringList>
#include <QtCore/QFile>




namespace Test {




bool ContractDetailsRequest::initialize( const IB::Contract& c )
{
	ibContract = c;
	return true;
}


bool ContractDetailsRequest::fromStringList( const QList<QString>& sl )
{
	ibContract.symbol = toIBString( sl[0] );
	ibContract.secType = toIBString( sl[1]);
	ibContract.exchange= toIBString( sl[2] );
	// optional filter for a single expiry
	QString e = sl.size() > 3 ? sl[3] : "";
	ibContract.expiry = toIBString( e );
	return true;
}








bool HistRequest::initialize( const IB::Contract& c, const QString &e,
	const QString &d, const QString &b,
	const QString &w, int u, int f )
{
	ibContract = c;
	endDateTime = e;
	durationStr = d;
	barSizeSetting = b;
	whatToShow = w;
	useRTH = u;
	formatDate = f;
	return true;
}


bool HistRequest::fromString( const QString& s )
{
	bool ok = false;
	QStringList sl = s.split('\t');
	
	if( sl.size() < 13 ) {
		return ok;
	}
	
	int i = 0;
	endDateTime = sl.at(i++);
	durationStr = sl.at(i++);
	barSizeSetting = sl.at(i++);
	whatToShow = sl.at(i++);
	useRTH = sl.at(i++).toInt( &ok );
	if( !ok ) {
		return false;
	}
	formatDate = sl.at(i++).toInt( &ok );
	if( !ok ) {
		return false;
	}
	
	ibContract.symbol = toIBString(sl.at(i++));
	ibContract.secType = toIBString(sl.at(i++));
	ibContract.exchange = toIBString(sl.at(i++));
	ibContract.currency = toIBString(sl.at(i++));
	ibContract.expiry = toIBString(sl.at(i++));
	ibContract.strike = sl.at(i++).toDouble( &ok );
	if( !ok ) {
		return false;
	}
	ibContract.right = toIBString(sl.at(i++));
	
	return ok;
}


QString HistRequest::toString() const
{
	QString c_str = QString("%1\t%2\t%3\t%4\t%5\t%6\t%7")
		.arg(toQString(ibContract.symbol))
		.arg(toQString(ibContract.secType))
		.arg(toQString(ibContract.exchange))
		.arg(toQString(ibContract.currency))
		.arg(toQString(ibContract.expiry))
		.arg(ibContract.strike)
		.arg(toQString(ibContract.right));
	
	QString retVal = QString("%1\t%2\t%3\t%4\t%5\t%6\t%7")
		.arg(endDateTime)
		.arg(durationStr)
		.arg(whatToShow)
		.arg(whatToShow)
		.arg(useRTH)
		.arg(formatDate)
		.arg(c_str);
	
	return retVal;
}


void HistRequest::clear()
{
	ibContract = IB::Contract();
	whatToShow.clear();
}








GenericRequest::GenericRequest() :
	reqType(NONE),
	reqState(FINISHED),
	reqId(0)
{
}


void GenericRequest::nextRequest( ReqType t )
{
	Q_ASSERT( reqState == FINISHED );
	reqType = t;
	reqState = PENDING;
	reqId++;
}








int HistTodo::fromFile( const QString & fileName )
{
	histRequests.clear();
	
	QList<QByteArray> rows;
	int retVal = read_file( fileName, &rows );
	if( retVal == -1) {
		return retVal;
	}
	foreach( QByteArray row, rows ) {
		if( row.startsWith('[') ) {
			int firstTab = row.indexOf('\t');
			Q_ASSERT( row.size() > firstTab );
			Q_ASSERT( firstTab >= 0 ); //TODO
			row.remove(0, firstTab+1 );
		}
		HistRequest hR;
		bool ok = hR.fromString( row );
		Q_ASSERT(ok); //TODO
		histRequests.append( hR );
	}
	return retVal;
}


int HistTodo::read_file( const QString & fileName, QList<QByteArray> *list ) const
{
	int retVal = -1;
	QFile f( fileName );
	if (f.open(QFile::ReadOnly)) {
		retVal = 0;
		while (!f.atEnd()) {
			QByteArray line = f.readLine();
			line.chop(1); //remove line feed
			if( line.startsWith('#') || line.isEmpty() ) {
				continue;
			}
			list->append(line);
			retVal++;
		}
	} else {
// 		_lastError = QString("can't read file '%1'").arg(fileName);
	}
	return retVal;
}


void HistTodo::dump( FILE *stream ) const
{
	for(int i=0; i < histRequests.size(); i++ ) {
		fprintf( stream, "[%d]\t%s\n",
		         i,
		         histRequests.at(i).toString().toUtf8().constData() );
	}
}








int ContractDetailsTodo::fromConfig( const QList< QList<QString> > &contractSpecs )
{
	int retVal = contractSpecs.size();
	foreach( const QList<QString> &sl, contractSpecs ) {
		ContractDetailsRequest cdR;
		bool ok = cdR.fromStringList( sl );
		Q_ASSERT(ok); //TODO
		contractDetailsRequests.append( cdR );
	}
	return retVal;
}




} // namespace Test