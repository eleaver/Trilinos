C Copyright(C) 1999-2020 National Technology & Engineering Solutions
C of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
C NTESS, the U.S. Government retains certain rights in this software.
C 
C See packages/seacas/LICENSE for details

C $Log: lnshow.f,v $
C Revision 1.3  2009/03/25 12:36:45  gdsjaar
C Add copyright and license notice to all files.
C Permission to assert copyright has been granted; blot is now open source, BSD
C
C Revision 1.2  1998/06/12 15:53:25  gdsjaar
C 1. Problem with TIMES array. Blot accesses a dummy timestep even if
C there were no timesteps on the database. Array wasn't allocated, so
C writing off into never-never land.
C
C 2. Inconsistency among some common blocks. Some places weren't using
C the include but had the definition hardwired in. Removed those.
C
C 3. Added 'EXTERNAL BLKDAT' to all routines that used data values set
C in BLKDAT
C
C 4. Cleanup of some A vs. IA argument passing.
C
C Revision 1.1  1994/04/07 20:04:33  gdsjaar
C Initial checkin of ACCESS/graphics/blotII2
C
c Revision 1.2  1990/12/14  08:53:07  gdsjaar
c Added RCS Id and Log to all files
c
C=======================================================================
      SUBROUTINE LNSHOW (SHOTYP, NAMES)
C=======================================================================

C   --*** LNSHOW *** (PATHLN) Display PATHLINE parameter information
C   --   Written by Amy Gilkey - revised 05/20/88
C   --
C   --LNSHOW displays the PATHLINE plot parameters.
C   --
C   --The SHOW options with the items they display are:
C   --   LOCATION - the pathlines to be plotted for the plot set
C   --   PLOT     -
C   --   HARDCOPY -
C   --
C   --Parameters:
C   --   SHOTYP - IN - the expanded SHOW option string
C   --   NAMES - IN - the variable names
C   --
C   --Common Variables:
C   --   Uses NLNCRV, ILVNE, ILVID of /LNVARS/

      include 'params.blk'
      include 'dbnums.blk'
      include 'lnvars.blk'

      CHARACTER*(*) SHOTYP
      CHARACTER*(*) NAMES(*)

      LOGICAL ISABRT
      CHARACTER*(MXNAME) NAM(3)
      CHARACTER TYP
      CHARACTER*2 STR2
      CHARACTER*8 STRA
      CHARACTER*80 STRING

      IF ((SHOTYP .EQ. 'LOCATION')
     &   .OR. (SHOTYP .EQ. 'PLOT') .OR. (SHOTYP .EQ. 'HARDCOPY')) THEN
         DO 110 NP = 1, NLNCRV
            IF (ISABRT ()) RETURN
            DO 100 IXY = 1, NDIM
               NAM(IXY) = NAMES(ILVID(IXY,NP))
  100       CONTINUE
            CALL DBVTYP_BL (ILVID(1,NP), TYP, IDUM)
            WRITE (STR2, '(I2)', IOSTAT=IDUM) NP
            WRITE (STRING, '(10A)') 'Pathline ', STR2, ' : ',
     &         (' ', NAM(I), I=1,NDIM), '^'
            LSTR = LENSTR (STRING) - 1
            IF ((TYP .EQ. 'N') .OR. (TYP .EQ. 'E')) THEN
               CALL INTSTR (1, 0, ILVNE(NP), STRA, L)
               IF (TYP .EQ. 'N') THEN
                  STRING(LSTR+1:) = ' Node ' // STRA(:L)
               ELSE
                  STRING(LSTR+1:) = ' Element ' // STRA(:L)
               END IF
            ELSE
               STRING(LSTR+1:LSTR+1) = ' '
            END IF
            WRITE (*, 10000) STRING(:LENSTR(STRING))
  110    CONTINUE

      END IF

      RETURN

10000  FORMAT (1X, 10A)
      END
