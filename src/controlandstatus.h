#ifndef _CONTROLANDSTATUS_H
#define _CONTROLANDSTATUS_H

#include <stdint.h>

typedef struct DP_REG_STAT DP_REG_STAT;
typedef struct AP_REG_STAT AP_REG_STAT;
typedef struct DP_ABORT DP_ABORT;

struct DP_REG_STAT{
  int OR_UNDETECT;			// (RoW)Overrun detection
  int STICKYORUN;		  		// (RoW)Check overrun status
  int TRNMODE_EN;	  		// (RoW)Set the transfer mode for AP operations
  int STICKYCMP;	    		// (RO) Set when there is a mismatch occurs during pushed-compare operation
  int STICKYERR;	    		// (RO) Set when an error is returned by AP transaction.
  int READ_OK;		        // (RoW) Response to the previous AP read or RDBUFF read
  int WDATA_ERR;		      // (RO) Set when write data error occurs
  int MASKLANE;		       //
  int ERR_MODE;	        // (RoW) Error mode, indicates reset behavoiur of ctrl/stat stickyerr field.

//#define	CDBGRSTREQ		1<<26		// Debug reset request
//#define	CDBGRSTACK		1<<27		// Debug reset acknowledge
//#define	CDBGPWRUPREQ	1<<28		// Debug powerup request
//#define CDBGPWRUPACK	1<<29		// Debug powerup acknowledge
//#define	CSYSPWRUPREEQ	1<<30		// System powerup request
//#define CSYSPWRUPACK	1<<31		// System powerup acknowledege
};

struct DP_ABORT{
  int ORUNERRCLR;		// Clear STICKYORUN	bit
  int WDERRCLR;	// Clear WDATAERR bit
  int STKERRCLR;		// Clear STICKYERR bit
  int STKCMPCLR;		// Clear STICKCMP bit
  int DAPABORT;	// Generate AP abort
};



#endif // _CONTROLANDSTATUS_H
