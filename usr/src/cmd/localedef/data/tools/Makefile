# $FreeBSD$
# See https://wiki.freebsd.org/LocaleNewApproach
# Taken from FreeBSD svn [base]/user/edwin/locale/cldr
#
# needs:
# devel/p5-Tie-IxHash
#
# Modified by John Marino to suit DragonFly needs
#

.if !defined(CLDRDIR)
CLDRDIR!=	grep ^cldr etc/unicode.conf | cut -f 2 -d " "
.endif
.if !defined(UNIDATADIR)
UNIDATADIR!=	grep ^unidata etc/unicode.conf | cut -f 2 -d " "
.endif
PASSON=		CLDRDIR="${CLDRDIR}" UNIDATADIR="${UNIDATADIR}"

ETCDIR=		${.CURDIR}/etc

KNOWN=		monetdef numericdef msgdef timedef colldef ctypedef
TYPES?=		${KNOWN}
LOCALE_DESTDIR?=	/tmp/generated-locales/

COLLATION_SPECIAL?= \
	cs_CZ ISO8859-2 \
	da_DK ISO8859-1 \
	da_DK ISO8859-15 \
	hr_HR ISO8859-2 \
	hu_HU ISO8859-2 \
	sq_AL ISO8859-2

.for area enc in ${COLLATION_SPECIAL}
COLLATIONS_SPECIAL_ENV+=	${area}.${enc}
.endfor
PASSON+=	COLLATIONS_SPECIAL="${COLLATIONS_SPECIAL_ENV}"

.if defined(LC)
LC:=	--lc=${LC}
.endif

all:
	cp ${ETCDIR}/common.UTF-8.src ${CLDRDIR}/posix/xx_Comm_US.UTF-8.src
.for t in ${TYPES}
.  if ${KNOWN:M${t}}
	test -d ${t} || mkdir ${t}
	${MAKE} build-${t}
.  endif
.endfor
	@echo ""
	@find . -name *failed

.for t in ${TYPES}
install: install-${t}
install-${t}:
.  if ${KNOWN:M${t}}
	rm -rf ${.CURDIR}/${t}.draft
	rm -rf ${.CURDIR}/../${t}
	mv ${.CURDIR}/${t} ${.CURDIR}/../${t}
.  endif
.endfor

post-install:
.for t in ${TYPES}
.  if ${KNOWN:M${t}}
	(cd ${.CURDIR}/../${t} && \
		${MAKE} && ${MAKE} install && ${MAKE} clean)
.  endif
.endfor

.for t in ${TYPES}
gen-${t}:
	mkdir -p ${t} ${t}.draft
	perl -I tools tools/cldr2def.pl \
		--cldr=$$(realpath ${CLDRDIR}) \
		--unidata=$$(realpath ${UNIDATADIR}) \
		--etc=$$(realpath ${ETCDIR}) \
		--type=${t} ${LC}

build-${t}: gen-${t}
	env ${PASSON} tools/finalize ${t}
.endfor

gen-ctypedef:	transfer-rollup
static-colldef: gen-colldef
build-colldef:	static-colldef

static-colldef:
.for area enc in ${COLLATION_SPECIAL}
	awk -f tools/extract-colldef.awk ${CLDRDIR}/posix/${area}.${enc}.src > colldef/${area}.${enc}.src
.endfor

transfer-rollup:
	cp ${ETCDIR}/common.UTF-8.src ${CLDRDIR}/posix/xx_Comm_US.UTF-8.src

rollup:
	perl -I tools tools/utf8-rollup.pl \
		--cldr=$$(realpath ${CLDRDIR}) \
		--etc=$$(realpath ${ETCDIR})

clean:
.for t in ${TYPES}
	rm -rf ${t} ${t}.draft
.endfor

BASE_LOCALES_OF_INTEREST?= \
	af_ZA am_ET ar_AE ar_BH ar_DZ ar_EG ar_IQ ar_JO ar_KW \
	ar_LB ar_LY ar_MA ar_OM ar_QA ar_SA ar_TN ar_YE \
	as_IN be_BY bg_BG bn_BD bo_CN ca_AD ca_ES ca_FR ca_IT \
	cs_CZ da_DK de_AT de_BE de_CH de_DE de_LI de_LU el_CY el_GR \
	en_AU en_BW en_BZ en_CA \
	en_GB en_HK en_IE en_IN en_JM en_MH en_MT en_NA en_NZ en_PH \
	en_PK en_SG en_TT en_US en_ZA en_ZW \
	es_AR es_BO es_CL es_CR es_CO es_DO es_EC es_ES es_GQ \
	es_GT es_HN es_MX es_NI es_PA es_PE es_PR es_PY es_SV \
	es_US es_UY es_VE et_EE \
	fi_FI fr_BE \
	fr_CA fr_CF fr_CH fr_FR fr_GN fr_LU fr_MC fr_MG fr_ML fr_NE \
	fr_SN ga_IE he_IL hi_IN hr_HR hu_HU hy_AM id_ID \
	is_IS it_CH it_IT ja_JP ko_KR kok_IN lt_LT lv_LV mk_MK mr_IN \
	mt_MT ne_IN ne_NP nb_NO nl_BE nl_NL nn_NO pl_PL \
	pt_BR pt_GW pt_MZ pt_PT ro_MD ro_RO \
	ru_MD ru_RU ru_UA se_FI se_NO sk_SK sl_SI sq_AL sv_FI sv_SE \
	ta_LK tr_TR uk_UA ur_IN ur_PK \
	az_Latn_AZ bs_Latn_BA kk_Cyrl_KZ mn_Cyrl_MN \
	ms_Latn_MY sr_Cyrl_RS sr_Latn_BA sr_Latn_ME sr_Latn_RS ug_Arab_CN \
	zh_Hans_CN zh_Hans_MO zh_Hans_SG zh_Hant_HK zh_Hant_TW \
	bn_IN gu_IN or_IN ta_IN te_IN kn_IN ml_IN si_LK \
	th_TH lo_LA bo_IN my_MM pa_Arab_PK pa_Guru_IN ka_GE chr_US \
	km_KH shi_Tfng_MA ii_CN vai_Vaii_LR vi_VN

ENCODINGS=	GB18030 \
		ISO8859-1 \
		ISO8859-13 \
		ISO8859-15 \
		ISO8859-2 \
		ISO8859-5 \
		ISO8859-7 \
		ISO8859-9 \
		KOI8-R \
		UTF-8

POSIX:
.if exists (${CLDRDIR}/tools/java/cldr.jar)
	mkdir -p ${CLDRDIR}/posix
.  for area in ${BASE_LOCALES_OF_INTEREST}
.    if !exists(${CLDRDIR}/posix/${area}.UTF-8.src)
	java -DCLDR_DIR=${CLDRDIR:Q} -jar ${CLDRDIR}/tools/java/cldr.jar \
		org.unicode.cldr.posix.GeneratePOSIX \
		-d ${CLDRDIR}/posix -m ${area} -c UTF-8
.    endif
.  endfor
.  for area encoding in ${COLLATION_SPECIAL}
.    if !exists(${CLDRDIR}/posix/${area}.${encoding}.src)
	java -DCLDR_DIR=${CLDRDIR:Q} -jar ${CLDRDIR}/tools/java/cldr.jar \
		org.unicode.cldr.posix.GeneratePOSIX \
		-d ${CLDRDIR}/posix -m ${area} -c ${encoding}
.    endif
.  endfor
.  for enc in ${ENCODINGS}
.  if !exists(${CLDRDIR}/posix/${enc}.cm)
	java -DCLDR_DIR=${CLDRDIR:Q} -jar ${CLDRDIR}/tools/java/cldr.jar \
		org.unicode.cldr.posix.GenerateCharmap \
		-d ${CLDRDIR}/posix -c ${enc}
.  endif
.  endfor
.else
	@echo "Please install CLDR toolset for the desired release"
	@echo "It should go at ${CLDRDIR}/tools"
.endif

clean-POSIX:
	rm -f ${CLDRDIR}/posix/*
