/*
 * Copyright (C) 2019  SCS Lab <scs-help@cs.iit.edu>, Hariharan
 * Devarajan <hdevarajan@hawk.iit.edu>, Luke Logan
 * <llogan@hawk.iit.edu>, Xian-He Sun <sun@iit.edu>
 *
 * This file is part of HCompress
 *
 * HCompress is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <data_structures.h>
/**
 * Output streams for my data structures
 */
/*
 * initializes the static attributes within Layer class.
 */
Layer* Layer::FIRST = nullptr; /* ram */
Layer* Layer::LAST = nullptr;  /* pfs */

std::ostream &operator<<(std::ostream &os, char const *m) {
	return os << std::string(m);
}
std::ostream &operator<<(std::ostream &os, uint8_t const &m) {
	return os << std::to_string(m);
}
std::ostream &operator<<(std::ostream &os, Layer const &m) {
	return os   << "id_:" << m.id_ << ","
	            << "capacity_mb_:" << m.capacity_mb_ << ","
	            << "layer_loc:" << std::string(m.layer_loc.c_str()) << ","
	            << "bandwidth_mbps_:" << m.bandwidth_mbps_ << ","
	            << "io_client_type:" << m.io_client_type<<"}";
}

std::ostream &operator<<(std::ostream &os, Input const &m) {
	return os   << "{operation_type:" << m.operation_type <<","
	            << "layer:" << m.layer<<"}";
}

std::ostream &operator<<(std::ostream &os, ReadInput const &m) {
	return os   << m;
}

std::ostream &operator<<(std::ostream &os, WriteInput const &m) {
	return os   << m;
}
std::ostream &operator<<(std::ostream &os, std::vector<hsize_t> const &m) {
	for (auto i = m.begin(); i != m.end(); ++i)
		os <<"["<< *i << ',';
	os <<"]";
	return os;
}

std::ostream &operator<<(std::ostream &os, bip::vector<hsize_t> const &m) {
	for (auto i = m.begin(); i != m.end(); ++i)
		os <<"["<< *i << ',';
	os <<"]";
	return os;
}

std::ostream &operator<<(std::ostream &os, std::array<hsize_t,MAX_DIMS> const &m) {
	for (auto i = m.begin(); i != m.end(); ++i)
		os <<"["<< *i << ',';
	os <<"]";
	return os;
}

std::ostream &operator<<(std::ostream &os, HDF5Input const &m) {
	return os   <<"{operation_type:" << m.operation_type <<","
	            << "layer:" << m.layer<<","
	            << "filename:" << m.filename <<","
	            << "dataset_name:" << m.dataset_name <<","
	            << "rank_:" << m.rank_ <<","
	            << "type_:" << m.type_ <<","
	            << "dimentions_:" << m.dimentions_ <<","
	            << "max_dimentions_:" << m.max_dimentions_ <<","
	            << "start_:" << m.file_start_ <<","
	            << "end_:" << m.file_end_ <<"}";
}
std::ostream &operator<<(std::ostream &os, HDF5ReadInput const &m) {
	return operator<<(os,static_cast <const HDF5Input &>( m ));
}
std::ostream &operator<<(std::ostream &os, HDF5WriteInput const &m) {
	return operator<<(os,static_cast <const HDF5Input &>( m ));
}

std::ostream &operator<<(std::ostream &os, Output const &m) {
	return os   << "";
}

std::ostream &operator<<(std::ostream &os, WriteOutput const &m) {
	return os   << "";
}
std::ostream &operator<<(std::ostream &os, ReadOutput const &m) {
	return os   << "";
}
std::ostream &operator<<(std::ostream &os, HDF5Output const &m) {
	return os   << "{error:"<<m.error<<"}";
}
std::ostream &operator<<(std::ostream &os, HDF5WriteOutput const &m) {
	return operator<<(os,static_cast <const HDF5Output &>( m ));
}

std::ostream &operator<<(std::ostream &os, HDF5ReadOutput const &m) {
	return operator<<(os,static_cast <const HDF5Output &>( m ));
}
template <typename I,typename O,
		typename std::enable_if<std::is_base_of<Input, I>::value>::type*,
		typename std::enable_if<std::is_base_of<Output, O>::value>::type*>
std::ostream &operator<<(std::ostream &os, Placement<I,O> const &m) {
	return os   << "{source_info:"<<m.source_info<<","
	            << "destination_info:"<<m.destination_info<<","
	            << "user_info:"<<m.user_info<<","
	            << "output:"<<m.output<<","
	            << "score_:"<<m.score_<<","
	            << "move_required_:"<<m.move_required_<<"}";
}
std::ostream &operator<<(std::ostream &os, Data const &m) {
	return os   << "";
}

std::ostream &operator<<(std::ostream &os, Matrix const &m) {
	return os   << "{num_dimension:"<<m.num_dimension<<","
	            << "start_:"<<m.start_<<","
	            << "original_start_:"<<m.original_start_<<","
	            << "end_:"<<m.end_<<"}";
}
