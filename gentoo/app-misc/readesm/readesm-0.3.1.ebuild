# Copyright 1999-2009 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI="2"

DESCRIPTION="digital tachograph file reader"
HOMEPAGE="http://readesm.sourceforge.net/"
SRC_URI="mirror://sourceforge/${PN}/${P}.tar.bz2"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~amd64"
IUSE=""

DEPEND=">=dev-libs/boost-1.35.0
	>=dev-libs/gmp-4.2.4
	>=dev-libs/libgcrypt-1.4.4"

RDEPEND="${DEPEND}"

src_compile() {
	cd ${WORKDIR}/${PN}
	emake || die "build failed"
}

src_install() {
	cd ${WORKDIR}/${PN}
	emake prefix=${D}/usr install || die "install failed"
}
