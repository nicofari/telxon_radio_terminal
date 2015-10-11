/* Addition information needed to interface Radio Packet Drivers */

/* Packet driver function call numbers.  */
#define GET_CONFIGURATION			128				/* set configuration */
#define GOTO_SLOW_POLL				129				/* goto slow poll command */
#define GET_RADIO_STATISTICS  130       /* get radio statistics   */
#define RESTART               131       /* reset/configure radio */

#ifndef uchar
#define uchar	unsigned char
#define ushort	unsigned int
#define ulong  unsigned long
#endif

/*		SPREAD SPECTRUM RADIO CONFIGURATION			    */
typedef struct
{		
	ushort	TxTimeout;						/*0x100*/
  ushort  TransportTime;        /*0x102*/
	uchar		FILL0[0x108-0x104];
  uchar   Irq;                  /*0x108*/
	uchar		SpreadCode;						/*0x109*/
	uchar		ChannelSet;						/*0x10A*/
	uchar		ChannelNum;						/*0x10B*/
	ushort	RadioNodeID;					/*0x10C*/
	uchar		FILL1[0x110-0x10e];
  uchar   ScramblingDisable;    /*0x110*/
	uchar		RadioType;						/*0x111*/
  ushort  RouterID;             /*0x112*/
	uchar		FILL12[0x11d-0x114];
  uchar   TXAttenuation;        /*0x11d*/
  ulong   SystemID;             /*0x11e*/
	ushort	GlobalChecksun;				/*0x122*/
	uchar		FILL3[0x128-0x124];
	ushort	MaxDatagram;					/*0x128*/
  ushort  MaxFrame;             /*0x12a*/
	uchar		MaxRetries;    				/*0x12c*/
	uchar		ReceiveMode;					/*0x12d*/
  uchar   Priority;             /*0x12e*/
	uchar		Root_Router;   				/*0x12f*/
  uchar   SpecifiedRouter[6];   /*0x130*/
  ushort  FastPollPeriod;       /*0x136*/
	uchar		PollDecay;						/*0x138*/
  ushort  FastPollDelay;        /*0x139*/
  uchar   ARL_Threshold;        /*0x13b*/
	uchar		ARL_Decay;						/*0x13C*/
	uchar		FILL6[0x145-0x13d];
	ulong		SystemIdOdd;					/*0x145*/
  uchar   EncrytionKey[14];     /*0x149*/
  ushort  WaitTime;             /*0x157*/
	ushort	L_Paramter;						/*0x159*/
	uchar		Fill7[0x15e-0x15b];
	ushort	HeaderSize;						/*0x15e*/
  ushort  SectionChecksum;      /*0x160*/
  uchar   RegistrationMode;     /*0x162*/
	uchar		FILL8;
  ushort  PollRate;             /*0x164*/
  ushort  RefreshRate;          /*0x166*/
  uchar   Name[16];             /*0x168*/
  uchar   NetworkID[6];         /*0x178*/
	uchar		FILL9;
  uchar   CodeFormat;           /*0x17f*/
  uchar   SScode[64];           /*0x180*/
	uchar		FILL10[0x200-0x1c0];
} SSPktCfg;

/*		SPREAD SPECTRUM RADIO STATISTICS	    */

typedef struct
{
  uchar   HostcpuLock;            /*0x300*/
  uchar   LancpuLock;             /*0x301*/
  uchar   ResetTime[18];          /*0x302*/
  ulong   PacketsSent;            /*0x314*/
  ulong   PacketsRetransmission;  /*0x318*/
  ulong   PacketsDiscarded;       /*0x31C*/
	ulong		PacketsReceived;  			/*0x320*/
	ulong		PacketsDuplicates;			/*0x324*/
  ulong   PacketsMailboxDiscard;  /*0x328*/
  ushort  MaxRetransmissionPkts;  /*0x32C*/
  ushort  MaxRetransmissionFrame; /*0x32E*/
  ushort  MaxDuplicates;          /*0x330*/
  ulong   BytesSent;              /*0x332*/
	ulong		BytesReceived;					/*0x336*/
  ulong   CRCErrors;              /*0x33A*/
  ulong   LengthErrors;           /*0x33E*/
  ulong   AbortErrors;            /*0x342*/
	ulong		TxUnderrunErrors; 			/*0x346*/
	ulong		RxOverrunErrors;  			/*0x34A*/
  ulong   HoldOffs;               /*0x34E*/
  ulong   FramesSent;             /*0x352*/
  ulong   FramesReceive;          /*0x356*/
	ulong		FramesReceiveLost;			/*0x35A*/
	ulong		RxOverflowsErrors;			/*0x35E*/
	ulong		AddressMismatch;  			/*0x362*/
  ulong   SidMismatch;            /*0x366*/
	ulong		PollsTransmitted; 			/*0x36A*/
	ulong		PollsAcknowledged;			/*0x36E*/
	ulong		StatusVectorTimeouts;		/*0x372*/
} SSPktStat;

/*		PACKET STATISTICS  */

typedef struct
{
	ulong		PacketsIn;
	ulong		PacketsOut;
	ulong		BytesIn;
	ulong		BytesOut;
	ulong		ErrorsIn;
	ulong		ErrorsOut;
	ulong		PacketDropped;
	ulong		NumStarted;
	ulong		NumStopped;
	ulong		NumTxErrors;
	ulong		NumAsTxErrors;
	ulong		TxStatus[10];
	uchar		LastError;
} PktStat;
