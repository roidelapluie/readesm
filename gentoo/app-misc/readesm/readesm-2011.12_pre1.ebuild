# Copyright 1999-2012 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI="2"

inherit cmake-utils

MY_P="${P/_pre/pre}"

DESCRIPTION="digital tachograph file reader"
HOMEPAGE="http://readesm.sourceforge.net/"
SRC_URI="mirror://sourceforge/${PN}/${MY_P}-Source.tar.bz2"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~amd64"
IUSE=""

DEPEND=">=dev-libs/boost-1.35.0
	>=dev-libs/gmp-5.0.2_p1
	>=dev-libs/libgcrypt-1.4.6
	>=x11-libs/qt-core-4.7.4
	>=x11-libs/qt-gui-4.7.4
	>=x11-libs/qt-webkit-4.7.4"

RDEPEND="${DEPEND}"

S="${WORKDIR}/${MY_P}-Source"

src_prepare () {
	find fileformat \( ! -regex '.*/\..*' ! -name "generate.py" \) | \
		xargs grep -snH "AUTO-GENERATED" --color | cut -f1 -d':' | \
		xargs rm
	cd fileformat
	python generate.py || die "could not prepare source: generate.py failed"
	cd -
}
