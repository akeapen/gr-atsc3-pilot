/*
 * Copyright 2023 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(subframemapper_cc.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(3be8f36f618cf79f0bd515d0666b45ee)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <atsc3/subframemapper_cc.h>
// pydoc.h is automatically generated in the build directory
#include <subframemapper_cc_pydoc.h>

void bind_subframemapper_cc(py::module& m)
{

    using subframemapper_cc    = ::gr::atsc3::subframemapper_cc;


    py::class_<subframemapper_cc, gr::block, gr::basic_block,
        std::shared_ptr<subframemapper_cc>>(m, "subframemapper_cc", D(subframemapper_cc))

        .def(py::init(&subframemapper_cc::make),
           py::arg("framesize1st"),
           py::arg("rate1st"),
           py::arg("fecmode1st"),
           py::arg("constellation1st"),
           py::arg("fftsize1st"),
           py::arg("numpayloadsyms1st"),
           py::arg("numpreamblesyms"),
           py::arg("guardinterval1st"),
           py::arg("pilotpattern1st"),
           py::arg("pilotboost1st"),
           py::arg("firstsbs"),
           py::arg("fimode1st"),
           py::arg("timode1st"),
           py::arg("tidepth1st"),
           py::arg("tiblocks1st"),
           py::arg("tifecblocksmax1st"),
           py::arg("tifecblocks1st"),
           py::arg("cred1st"),
           py::arg("framesize2nd"),
           py::arg("rate2nd"),
           py::arg("fecmode2nd"),
           py::arg("constellation2nd"),
           py::arg("fftsize2nd"),
           py::arg("numpayloadsyms2nd"),
           py::arg("guardinterval2nd"),
           py::arg("pilotpattern2nd"),
           py::arg("pilotboost2nd"),
           py::arg("fimode2nd"),
           py::arg("timode2nd"),
           py::arg("tidepth2nd"),
           py::arg("tiblocks2nd"),
           py::arg("tifecblocksmax2nd"),
           py::arg("tifecblocks2nd"),
           py::arg("cred2nd"),
           py::arg("paprmode"),
           py::arg("l1bmode"),
           py::arg("l1dmode"),
           D(subframemapper_cc,make)
        )
        



        ;




}








