/*!
 * \file interleaved_byte_to_complex_byte.cc
 * \brief Adapts an 8-bits interleaved sample stream into a 16-bits complex stream
 * \author Carles Fernandez Prades, cfernandez(at)cttc.es
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2015  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * GNSS-SDR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GNSS-SDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNSS-SDR. If not, see <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */


#include "interleaved_byte_to_complex_byte.h"
#include <gnuradio/io_signature.h>
#include <volk/volk.h>


interleaved_byte_to_complex_byte_sptr make_interleaved_byte_to_complex_byte()
{
    return interleaved_byte_to_complex_byte_sptr(new interleaved_byte_to_complex_byte());
}



interleaved_byte_to_complex_byte::interleaved_byte_to_complex_byte() : sync_decimator("interleaved_byte_to_complex_byte",
                        gr::io_signature::make (1, 1, sizeof(char)),
                        gr::io_signature::make (1, 1, sizeof(lv_8sc_t)), // lv_8sc_t is a Volk's typedef for std::complex<signed char>
                        2)
{
    const int alignment_multiple = volk_get_alignment() / sizeof(lv_8sc_t);
    set_alignment(std::max(1, alignment_multiple));
}


int interleaved_byte_to_complex_byte::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
{
    const signed char *in = (const signed char *) input_items[0];
    lv_8sc_t *out = (lv_8sc_t *) output_items[0];
    // This could be put into a Volk kernel
    unsigned int sample_index = 0;
    for(unsigned int number = 0; number < 2 * noutput_items; number++)
        {
            // lv_cmake(r, i) defined at volk/volk_complex.h
            *out++ = lv_cmake(in[sample_index], in[sample_index + 1]);
            sample_index = sample_index + 2;
        }

    return noutput_items;
}
