#
# Top level makefile for documentation build
#

BUILDDIR ?= _build
DOC_TAG ?= development
SPHINXOPTS ?= -j auto
KCONFIG_TURBO_MODE ?= 0

# Documentation targets
# ---------------------------------------------------------------------------

.PHONY: clean htmldocs htmldocs-fast pdfdocs doxygen

htmldocs-fast:
	${MAKE} htmldocs KCONFIG_TURBO_MODE=1

htmldocs pdfdocs doxygen: configure
	cmake --build ${BUILDDIR} -- $@  # -v # VERBOSE=1

# Run CMake every time cause it's quick and re-configures TURBO_MODE if
# needed
.PHONY: configure
configure:
	cmake -GNinja -B${BUILDDIR} -S. -DDOC_TAG=${DOC_TAG} \
		-DSPHINXOPTS="${SPHINXOPTS}" \
		-DKCONFIG_TURBO_MODE=${KCONFIG_TURBO_MODE}

clean:
	rm -rf ${BUILDDIR}
