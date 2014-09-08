/*!
 * \file galileo_e1_pcps_quicksync_ambiguous_acquisition.h
 * \brief Adapts a PCPS acquisition block to an AcquisitionInterface for Galileo E1 Signals
 * \date June, 2014
 * \author Damian Miralles Sanchez. dmiralles2009@gmail.com
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2014  (see AUTHORS file for a list of contributors)
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

#ifndef GNSS_SDR_GALILEO_E1_PCPS_QUICKSYNC_AMBIGUOUS_ACQUISITION_H_
#define GNSS_SDR_GALILEO_E1_PCPS_QUICKSYNC_AMBIGUOUS_ACQUISITION_H_

#include <string>
#include <gnuradio/msg_queue.h>
#include <gnuradio/blocks/stream_to_vector.h>
#include "gnss_synchro.h"
#include "acquisition_interface.h"
#include "pcps_quicksync_acquisition_cc.h"


class ConfigurationInterface;

/*!
 * \brief This class adapts a PCPS acquisition block to an
 *  AcquisitionInterface for Galileo E1 Signals
 */
class GalileoE1PcpsQuickSyncAmbiguousAcquisition: public AcquisitionInterface
{
public:
    GalileoE1PcpsQuickSyncAmbiguousAcquisition(ConfigurationInterface* configuration,
            std::string role, unsigned int in_streams,
            unsigned int out_streams, boost::shared_ptr<gr::msg_queue> queue);

    virtual ~GalileoE1PcpsQuickSyncAmbiguousAcquisition();

    std::string role()
    {
        return role_;
    }

    /*!
     * \brief Returns "Galileo_E1_PCPS_Ambiguous_Acquisition"
     */
    std::string implementation()
    {
        return "Galileo_E1_PCPS_QuickSync_Ambiguous_Acquisition";
    }
    size_t item_size()
    {
        return item_size_;
    }

    void connect(gr::top_block_sptr top_block);
    void disconnect(gr::top_block_sptr top_block);
    gr::basic_block_sptr get_left_block();
    gr::basic_block_sptr get_right_block();

    /*!
     * \brief Set acquisition/tracking common Gnss_Synchro object pointer
     * to efficiently exchange synchronization data between acquisition and
     *  tracking blocks
     */
    void set_gnss_synchro(Gnss_Synchro* p_gnss_synchro);

    /*!
     * \brief Set acquisition channel unique ID
     */
    void set_channel(unsigned int channel);

    /*!
     * \brief Set statistics threshold of PCPS algorithm
     */
    void set_threshold(float threshold);

    /*!
     * \brief Set maximum Doppler off grid search
     */
    void set_doppler_max(unsigned int doppler_max);

    /*!
     * \brief Set Doppler steps for the grid search
     */
    void set_doppler_step(unsigned int doppler_step);

    /*!
     * \brief Set tracking channel internal queue
     */
    void set_channel_queue(concurrent_queue<int> *channel_internal_queue);

    /*!
     * \brief Initializes acquisition algorithm.
     */
    void init();

    /*!
     * \brief Sets local code for Galileo E1 PCPS acquisition algorithm.
     */
    void set_local_code();

    /*!
     * \brief Returns the maximum peak of grid search
     */
    signed int mag();

    /*!
     * \brief Restart acquisition algorithm
     */
    void reset();

private:
    ConfigurationInterface* configuration_;
    pcps_quicksync_acquisition_cc_sptr acquisition_cc_;
    gr::blocks::stream_to_vector::sptr stream_to_vector_;
    size_t item_size_;
    std::string item_type_;
    unsigned int vector_length_;
    unsigned int code_length_;
    bool bit_transition_flag_;
    unsigned int channel_;
    float threshold_;
    unsigned int doppler_max_;
    unsigned int doppler_step_;
    unsigned int shift_resolution_;
    unsigned int sampled_ms_;
    unsigned int max_dwells_;
    unsigned int folding_factor_;
    long fs_in_;
    long if_;
    bool dump_;
    std::string dump_filename_;
    std::complex<float> * code_;
    Gnss_Synchro * gnss_synchro_;
    std::string role_;
    unsigned int in_streams_;
    unsigned int out_streams_;
    boost::shared_ptr<gr::msg_queue> queue_;
    concurrent_queue<int> *channel_internal_queue_;
    float calculate_threshold(float pfa);
};

#endif /* GNSS_SDR_GALILEO_E1_PCPS_QUICKSYNC_AMBIGUOUS_ACQUISITION_H_ */
