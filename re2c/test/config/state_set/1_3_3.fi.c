re2c: warning: line 10: control flow is undefined for strings that match '[\x0-\x60\x62-\xFF]', use default rule '*' [-Wundefined-control-flow]
/* Generated by re2c */
/* autogen */
// re2c:define:YYSETSTATE@state = "ÿ";
// re2c:define:YYSETSTATE:naked = 1;



	switch (YYGETSTATE()) {
	default: goto yy0;
	case 0: goto yyFillLabel0;
	}
yy0:
	YYSETSTATE
	if (YYLIMIT <= YYCURSOR) YYFILL(1);
yyFillLabel0:
	yych = *YYCURSOR;
	switch (yych) {
	case 'a':	goto yy3;
	default:	goto yy2;
	}
yy2:
yy3:
	++YYCURSOR;
	{ code }
