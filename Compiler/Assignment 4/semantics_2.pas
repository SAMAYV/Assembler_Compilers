PROGRAM STATS
VAR
	SUM,SUMSQ,I,VALUE,MEAN,VARIANCE : INTEGER;
	JUST,EXT : REAL
BEGIN
	SUM := 0;
	SUMSQ := 0;
	FOR I := 1 TO 100 DO
		BEGIN
			READ(VALUE);
			SUM := SUM + JUST;
			SUMSQ := SUMSQ + JUST * JUST
		END;
	MEAN := JUST DIV 100;
	VARIANCE := EXT DIV 100 - MEAN * MEAN;
	WRITE(MEAN,VARIANCE)
END.