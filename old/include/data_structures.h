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


#ifndef SCORE_DATA_STRUCTURES_H
#define SCORE_DATA_STRUCTURES_H


#include <string>
#include <H5public.h>
#include <hdf5.h>
#include <enum.h>
//#include <constants.h>
#include <vector>
#include <cstdint>
#include <rpc/msgpack.hpp>
#include <chrono>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
//#include <basket/common/data_structures.h>
//#include <basket/common/typedefs.h>

#ifdef ENABLE_COMPRESSION
#include <ares/c++/ares.h>
#endif

using namespace std;
namespace bip=boost::interprocess;

/**
 * This class represents the various hierarchical layers supported hermes platform.
 */
class Layer {
public:
	/**
	 * Attributes
	 */
	static Layer *FIRST; /* define enum for First layer */
	static Layer *LAST; /* define enum for Last Layer */
	uint8_t id_; /* id of the layer. */
	float capacity_mb_; /* capacity in Megabytes of the layer. */
	CharStruct layer_loc; /* location of the layer. */
	float bandwidth_mbps_; /* bandwidth in Megabytes per second. */
	Layer *next; /* pointer to next layer. */
	Layer *previous; /* pointer to previous layer. */
	bool direct_io;
	IOClientType io_client_type; /* Type of IO Client to use for the layer*/
	/*MSGPACK_DEFINE (id_, capacity_mb_, layer_loc, bandwidth_mbps_, direct_io, io_client_type)*/

	/**
	 * Constructors
	 */
	Layer() : layer_loc() {} /* default constructor */
	Layer(const Layer &other) :
			id_(other.id_),
			capacity_mb_(other.capacity_mb_),
			layer_loc(other.layer_loc),
			bandwidth_mbps_(other.bandwidth_mbps_),
			next(other.next),
			previous(other.previous),
			direct_io(other.direct_io),
			io_client_type(other.io_client_type) {} /* copy constructor */
	/* parameterized constructor */
	Layer(Layer *next,
	      Layer *previous,
	      bool is_present,
	      uint8_t id,
	      float capacity,
	      CharStruct layer_loc,
	      float bandwidth,
	      IOClientType io_client_type) :
			id_(id),
			capacity_mb_(capacity),
			layer_loc(layer_loc),
			next(next),
			previous(previous),
			bandwidth_mbps_(bandwidth),
			io_client_type(io_client_type) {
	}
	/**
	 * Operators
	 */
	/* equal operations for two layers. */
	bool operator==(const Layer &o) const {
		return id_ == o.id_;
	}

	/* not-equal operations for two layers. */
	bool operator!=(const Layer &o) const {
		return id_ != o.id_;
	}
};

/**
 * Input abstract structure to build multiple buffer interfaces easily through templating
 */
typedef struct Input {
	/**
	 * attributes
	 */
	OperationType operation_type; /* defines operation type for input (e.g. Read/ Write). */
	Layer layer; /* defines the input's source layer (e.g. ram, nvme, bb, or, pfs. */
	void *buffer; /* defines the input buffer. */
	CompressionLibrary comp_lib; /*Defines compression used for input*/
	size_t comp_size; /*Defines the size of the compressed data*/

	/**
	 * constructors
	 */
	Input() : buffer(), layer(*Layer::LAST) {} /* default constructor */
	Input(OperationType operation_type) : operation_type(operation_type),
	                                      buffer(),
	                                      layer(*Layer::LAST), comp_lib(CompressionLibrary::DUMMY),
	                                      comp_size(0) {} /* parameterized constructor */
	Input(OperationType operation_type, Layer layer) : operation_type(operation_type),
	                                                   layer(layer),
	                                                   buffer(),
	                                                   comp_lib(CompressionLibrary::DUMMY),
	                                                   comp_size(0) {}  /* parameterized constructor */
	Input(const Input &other) : operation_type(other.operation_type),
	                            layer(other.layer),
	                            buffer(other.buffer),
	                            comp_lib(other.comp_lib),
	                            comp_size(other.comp_size) {}  /* copy constructor */

} Input;
/**
 * ReadInput structure is a generic struct for Read input APIs. It is used for generic templating
 */
typedef struct ReadInput : public Input {
	/**
	 * constructors
	 */
	ReadInput() : Input(OperationType::READ) {} /* default constructor */
} ReadInput;


/**
 * WriteInput structure is a generic struct for Write input APIs. It is used for generic templating
 */
typedef struct WriteInput : public Input {
	/**
	 * constructors
	 */
	WriteInput() : Input(OperationType::WRITE) {} /* default constructor */
} WriteInput;

/**
 * HDF5Input structure is a generic struct for HDF5 input APIs.
 * It describes basic input to encapsulate the dataset information.<br>
 * It includes attributes such as:
 * <ul>
 *      <li>filename:string</li>
 *      <li>dataset_name:string</li>
 *      <li>dimentions_:uint32_t[]</li>
 *      <li>start_:uint32_t[]</li>
 *      <li>end_:uint32_t[]</li>
 * </ul>
 */
typedef struct HDF5Input : public Input {
	/**
	 * attributes
	 */
	CharStruct filename; /* filename of HDF5 file */
	CharStruct dataset_name; /* dataset within HDF5 file */
	int rank_; /* rank of HDF5 dataset */
	int64_t type_; /* data type of HDF5 dataset */
	std::array<hsize_t, MAX_DIMS> dimentions_; /* dimentions of HDF5 dataset */
	std::array<hsize_t, MAX_DIMS> max_dimentions_; /* max dimentions of HDF5 dataset */
	std::array<hsize_t, MAX_DIMS> file_start_; /* start offsets of HDF5 dataset */
	std::array<hsize_t, MAX_DIMS> file_end_; /* end offsets of HDF5 dataset */
	std::array<hsize_t, MAX_DIMS> memory_start_; /* start offsets of HDF5 dataset */
	std::array<hsize_t, MAX_DIMS> memory_dim_; /* end offsets of HDF5 dataset */
	hid_t vol_id; /* dataset id of the PFS file*/
	void *dataset_object; /* dataset id of the PFS file*/
	void *file_object; /* file id of the PFS file*/
	/*MSGPACK_DEFINE_ARRAY(filename, dataset_name, rank_, type_, dimentions_, max_dimentions_, file_start_, file_end_,
						 memory_start_, memory_dim_, dataset_id, file_id, MSGPACK_BASE(Input));*/

	/**
	 * Constructors
	 */
	HDF5Input(OperationType operation_type) : Input(operation_type), filename(), dataset_name(), rank_(0),
	                                          dimentions_(), max_dimentions_(), file_start_(), file_end_(),
	                                          memory_start_(),
	                                          memory_dim_() {} /* parameterized constructor */
	HDF5Input()
			: filename(), dataset_name(), rank_(0), dimentions_(), max_dimentions_(), file_start_(), file_end_(),
			  memory_start_(),
			  memory_dim_() {} /* default constructor */
	HDF5Input(const HDF5Input &other) : Input(other),
	                                    filename(other.filename),
	                                    dataset_name(other.dataset_name),
	                                    rank_(other.rank_),
	                                    type_(other.type_),
	                                    dimentions_(other.dimentions_),
	                                    max_dimentions_(other.max_dimentions_),
	                                    file_start_(other.file_start_),
	                                    file_end_(other.file_end_),
	                                    memory_start_(other.memory_start_),
	                                    memory_dim_(other.memory_dim_),
	                                    dataset_object(other.dataset_object),
	                                    file_object(other.file_object),
	                                    vol_id(other.vol_id) {} /* copy constructor */

	HDF5Input(HDF5Input &&other) : Input(other),
	                               filename(other.filename),
	                               dataset_name(other.dataset_name),
	                               rank_(other.rank_),
	                               type_(other.type_),
	                               dimentions_(std::move(other.dimentions_)),
	                               max_dimentions_(std::move(other.max_dimentions_)),
	                               file_start_(other.file_start_),
	                               file_end_(other.file_end_),
	                               memory_start_(other.memory_start_),
	                               memory_dim_(other.memory_dim_),
	                               dataset_object(other.dataset_object),
	                               file_object(other.file_object),
	                               vol_id(other.vol_id) {} /* move constructor*/
	HDF5Input &operator=(const HDF5Input &other) {
		Input::operator=(other);
		filename = other.filename;
		dataset_name = other.dataset_name;
		rank_ = other.rank_;
		type_ = other.type_;
		dimentions_ = other.dimentions_;
		max_dimentions_ = other.max_dimentions_;
		file_start_ = other.file_start_;
		file_end_ = other.file_end_;
		memory_start_ = other.memory_start_;
		memory_dim_ = other.memory_dim_;
		dataset_object = other.dataset_object;
		file_object = other.file_object;
		vol_id = other.vol_id;
		return *this;
	}
	/**
	 * Functions
	 */
	/**
	 * This function counts the length of each dimention and returns the count.
	 *
	 * @return vector of counts of each dimention in the dataset
	 */
	std::array<hsize_t, MAX_DIMS> GetCount() {
		std::array<hsize_t, MAX_DIMS> count = std::array<hsize_t, MAX_DIMS>();
		for (int i = 0; i < rank_; i++) {
			count[i] = (file_end_[i] - file_start_[i] + 1);
		}
		return count;
	}

	/**
	 * This function get the total size of dataset in bytes.
	 *
	 * @return size of dataset.
	 */
	uint64_t GetSize() {
		uint64_t size = 1;
		for (int i = 0; i < rank_; i++) {
			size *= (file_end_[i] - file_start_[i] + 1);
		}
		return size * H5Tget_size(type_);
	}

	/**
	 * This function get the total size of dataset in bytes.
	 *
	 * @return size of dataset.
	 */
	uint64_t GetDimentionSize() {
		uint64_t size = 1;
		for (int i = 0; i < rank_; i++) {
			size *= dimentions_[i] * H5Tget_size(type_);
		}
		return size;
	}


} HDF5Input;


/**
 * HDF5ReadInput structure is a struct for HDF5 Read inputs APIs.
 * It describes input to encapsulate the dataset  read information.
 * It inherits its features from HDF5Input and ReadInput
 */
typedef struct HDF5ReadInput : public HDF5Input, public ReadInput {
	/**
	 * constructors
	 */
	HDF5ReadInput() : HDF5Input::HDF5Input(OperationType::READ),
	                  ReadInput::ReadInput() {} /* default constructors */
	HDF5ReadInput(HDF5Input &other) : HDF5Input(other), ReadInput() {} /* clone constructors */
} HDF5ReadInput;
/**
 * HDF5WriteInput structure is a struct for HDF5 Write inputs APIs.
 * It describes input to encapsulate the dataset  write information.
 * It inherits its features from HDF5Input and WriteInput
 */
typedef struct HDF5WriteInput : public HDF5Input, public WriteInput {
	/**
	 * constructors
	 */
	HDF5WriteInput() : HDF5Input::HDF5Input(OperationType::WRITE),
	                   WriteInput::WriteInput() {} /* default constructor */
	HDF5WriteInput(HDF5Input &other) : HDF5Input(other), WriteInput() {} /* clone constructors */
	HDF5WriteInput(const HDF5WriteInput &other) : HDF5Input(other), WriteInput() {} /* copy constructors */

} HDF5WriteInput;
/**
 * Output abstract structure to build multiple buffer interfaces easily through templating
 */
typedef struct Output {
} Output;
/**
 * WriteOutput structure is a generic struct for Write output APIs. It is used for generic templating
 */
typedef struct WriteOutput : public Output {
	/**
	 * constructors
	 */
	WriteOutput(WriteOutput &out) {} /* copy constructor */
	WriteOutput() {} /* default constructor */
} WriteOutput;

/**
 * ReadOutput structure is a generic struct for Read output APIs. It is used for generic templating
 */
typedef struct ReadOutput : public Output {
} ReadOutput;
/**
 * HDF5Output abstract structure to build multiple buffer interfaces easily through templating
 */

typedef struct HDF5Output : public Output {
	/**
	 * Attributes
	 */
	herr_t error; /* error for storing HDF5 error codes.*/
	/**
	 * Constructors
	 */
	HDF5Output(const HDF5Output &out) : error(out.error), Output() {} /* copy constructor */
	HDF5Output() : Output(), error(0) {} /* default constructor */
	HDF5Output(herr_t error) : error(error) {} /* parameterized constructor */
	/**
	 * Methods
	 */
	/**
	 * This method checks if the output returned a success or not.
	 *
	 * @return true or false
	 */
	bool CheckSuccess() {
		return error >= 0;
	}
} HDF5Output;

/**
* HDF5WriteOutput structure is a struct for HDF5 Write output for APIs.
* It describes output to encapsulate the dataset  write information.
* It inherits its features from HDF5Output and WriteOutput
*/
struct HDF5WriteOutput : public HDF5Output, WriteOutput {
	/**
	 * Constructors
	 */
	HDF5WriteOutput() : HDF5Output(), WriteOutput() {} /* default constructor */
	HDF5WriteOutput(HDF5WriteOutput &other) : HDF5Output(other.error), WriteOutput() {} /* copy constructor */
	HDF5WriteOutput(HDF5WriteOutput &&other) : HDF5Output(other.error), WriteOutput() {} /* move constructor */
	/**
	 * Operators
	 */
	/* Assignment operator HDF5WriteOutput to HDF5WriteOutput */
	HDF5WriteOutput &operator=(const HDF5WriteOutput &other) {
		this->error = other.error;
	}

	/* Assignment operator HDF5Output to HDF5WriteOutput */
	HDF5WriteOutput &operator=(const HDF5Output &other) {
		this->error = other.error;
	}

	/* Addition operator HDF5Output to HDF5WriteOutput */
	HDF5WriteOutput &operator+(const HDF5Output &other) {
		if (HDF5Output(other).CheckSuccess())
			return *this;

		HDF5WriteOutput t = *this;
		t.error += other.error;
		return t;
	}
};

/**
* HDF5ReadOutput structure is a struct for HDF5 Read output for APIs.
* It describes output to encapsulate the dataset  read information.
* It inherits its features from HDF5Output and ReadOutput
*/
struct HDF5ReadOutput : public HDF5Output, ReadOutput {
	/**
	 * Constructors
	 */
	HDF5ReadOutput() : HDF5Output(), ReadOutput() {} /* default constructor */
	HDF5ReadOutput(HDF5ReadOutput &other) : HDF5Output(other.error), ReadOutput() {} /* copy constructor */
	HDF5ReadOutput(HDF5ReadOutput &&other) : HDF5Output(other.error), ReadOutput() {} /* move constructors */
	/**
	 * Operators
	 */
	/* Assignment operator HDF5ReadOutput to HDF5ReadOutput */
	HDF5ReadOutput &operator=(const HDF5ReadOutput &other) {
		this->error = other.error;
	}

	/* Assignment operator HDF5Output to HDF5ReadOutput */
	HDF5ReadOutput &operator=(const HDF5Output &other) {
		this->error = other.error;
	}

	/* Addition operator HDF5Output to HDF5ReadOutput */
	HDF5ReadOutput &operator+(const HDF5Output &other) {
		if (HDF5Output(other).CheckSuccess())
			return *this;

		HDF5ReadOutput t = *this;
		t.error += other.error;
		return t;
	}
};


/**
 * This structure holds the placement of the data. It holds attributes like
 * <ul>
 *  <li> source_info </li>
 *  <li> destination_info </li>
 *  <li> user_info </li>
 *  <li> output </li>
 *  <li> score </li>
 *  <li> move_required_ </li>
 * </ul>
 * @tparam I
 * @tparam O
 */
template<typename I, typename O,
		typename std::enable_if<std::is_base_of<Input, I>::value>::type * = nullptr,
		typename std::enable_if<std::is_base_of<Output, O>::value>::type * = nullptr>
struct Placement {
	/**
	 * Attributes
	 */
	I source_info; /* information of source of the placement (e.g. memory for write and file for read). */
	I destination_info; /* information of destination of the placement (e.g. memory for read and file for write). */
	I user_info; /* information of user's placement. */
	O output; /* output contains the error codes for performing placement. */
	float score_; /* holds the score of the placement. */
	bool move_required_; /* holds if placement requires making of space in the layer. */

	/**
	 * Constructors
	 */
	Placement() {} /* default constructor */
	Placement(Placement<I, O> &&other) : source_info(other.source_info),
	                                     destination_info(other.destination_info),
	                                     user_info(other.user_info),
	                                     output(other.output),
	                                     score_(other.score_),
	                                     move_required_(other.move_required_) {} /* move constructor */
	Placement(const Placement<I, O> &other) : source_info(other.source_info),
	                                          destination_info(other.destination_info),
	                                          user_info(other.user_info),
	                                          output(other.output),
	                                          score_(other.score_),
	                                          move_required_(other.move_required_) {} /* copy constructor */
	/**
	 * Operators
	 */
	/* Assignment operator from Placement to Placement. */
	Placement<I, O> operator=(const Placement<I, O> &other) {
		Placement<I, O> p(other);
		return p;
	}

};

/*
 * This holds the generic representation of data (e.g. offsets and size for POSIX or Matrix for HDF5 Datasets.
 */
struct Data {
	/**
	 * Constructors
	 */
	Data() {} /* default constructor. */
	Data(const Data &other) {} /* copy constructor. */
	/**
	 * Operators
	 */
	/* Equal operator for two Data Objects. */
	bool operator==(const Data &o) const {
		return true;
	}
};

/**
 * This structure holds format of a matrix. This is useful to depict an n-dimentional dataset.
 */
struct Matrix : public Data {
	/**
	 * Attributes
	 */
	hsize_t num_dimension; /* holds the number of dimension. */
	std::array<hsize_t, MAX_DIMS> start_; /* holds the start offset per dimension. */
	std::array<hsize_t, MAX_DIMS> original_start_; /* holds the start offset per dimension when file was created */
	std::array<hsize_t, MAX_DIMS> end_; /* holds the end offset per dimension. */
	//MSGPACK_DEFINE (num_dimension, start_, original_start_, end_);

	/**
	 * Constructors
	 */
	Matrix() : num_dimension(0), start_(), end_(), original_start_() {} /* default constructor */
	Matrix(const Matrix &other) : num_dimension(other.num_dimension),
	                              start_(other.start_),
	                              original_start_(other.original_start_),
	                              end_(other.end_) {} /* copy constructor*/
	Matrix(Matrix &&other) : num_dimension(other.num_dimension),
	                         start_(other.start_),
	                         original_start_(other.original_start_),
	                         end_(other.end_) {} /* move constructor*/
	Matrix(hsize_t rank, std::array<hsize_t, MAX_DIMS> start_, std::array<hsize_t, MAX_DIMS> end_) : start_(start_),
	                                                                                                 num_dimension(
			                                                                                                 rank),
	                                                                                                 end_(end_),
	                                                                                                 original_start_() {} /* parameterized constructor */


	/**
	 * Operators
	 */
	Matrix &operator=(const Matrix &other) {
		num_dimension = other.num_dimension;
		start_ = other.start_;
		original_start_ = other.original_start_;
		end_ = other.end_;
		return *this;
	}

	/* less than operator for comparing two Matrix. */
	bool operator<(const Matrix &o) const {
		hsize_t volume = 1;
		hsize_t volume_o = 1;
		for (hsize_t i = 0; i < num_dimension; i++) {
			if (o.start_[i] < start_[i])
				return false;
			else if (o.start_[i] > start_[i])
				return true;
		}
		if (volume_o < volume)
			return false;
		return true;
	}

	/* greater than operator for comparing two Matrix. */
	bool operator>(const Matrix &o) const {
		return o < *this;
	}

	/* equal operator for comparing two Matrix. */
	bool operator==(const Matrix &o) const {
		if (num_dimension == 0 || o.num_dimension == 0) return false;
		for (hsize_t i = 0; i < num_dimension; i++) {
			if (start_[i] != o.start_[i] || end_[i] != o.end_[i]) return false;
		}
		return true;
	}

	/* not equal operator for comparing two Matrix. */
	bool operator!=(const Matrix &o) const {
		return !(*this == o);
	}


	/**
	 * Methods
	 */
	/**
	 * This methods initializes the matrix with default values for a given rank.
	 *
	 * @param rank
	 */
	void GetDefault(hsize_t rank) {
		this->num_dimension = rank;
		for (auto r = 0; r < rank; r++) {
			this->start_[r] = 0;
			this->end_[r] = 0;
		}
	}

	/**
	 * Checks if the given current matrix contains the given matrix.
	 *
	 * @param o, given matrix
	 * @return true if it contains else false.
	 */
	bool Contains(const Matrix &o) const {
		bool contains = true;
		for (hsize_t i = 0; i < num_dimension; i++) {
			contains = contains && ((start_[i] <= o.start_[i] && end_[i] >= o.start_[i]) ||
			                        (start_[i] <= o.end_[i] && end_[i] >= o.end_[i]));
		}
		return contains;
	}

	/**
	 * This method calculates A - B of Matrix. A is this matrix
	 * @param o, B matrix
	 * @return vectors of matrix which are left over from the substraction.
	 */
	bip::vector<Matrix> Substract(const Matrix &o) const {
		bip::vector<Matrix> left_over = bip::vector<Matrix>();
		Matrix myinstance(*this);
		for (hsize_t i = 0; i < num_dimension; i++) {
			if (start_[i] < o.start_[i] && end_[i] >= o.start_[i]) {
				Matrix new_matrix(myinstance);
				if (original_start_.size() == 0)
					new_matrix.original_start_ = this->start_;
				new_matrix.end_[i] = o.start_[i] - 1;
				left_over.push_back(new_matrix);
				myinstance.start_[i] = o.start_[i];
			}
			if (start_[i] <= o.end_[i] && end_[i] > o.end_[i]) {
				Matrix new_matrix(myinstance);
				if (original_start_.size() == 0)
					new_matrix.original_start_ = this->start_;
				new_matrix.start_[i] = o.end_[i] + 1;
				left_over.push_back(new_matrix);
				myinstance.end_[i] = o.end_[i];
			}
		}
		return left_over;
	}

	/**
	 * Calculates the intersection of two matrix.
	 * @param o, is the given matrix
	 * @return intersection of two matrix.
	 */
	Matrix Intersect(Matrix &o) const {
		Matrix intersection(*this);
		for (hsize_t i = 0; i < num_dimension; i++) {
			if (start_[i] < o.start_[i]) {
				intersection.start_[i] = o.start_[i];
			} else {
				intersection.start_[i] = start_[i];
			}
			if (end_[i] < o.end_[i]) {
				intersection.end_[i] = end_[i];
			} else {
				intersection.end_[i] = o.end_[i];
			}
		}
		return intersection;
	}
};

typedef struct FileInfo {
	uint32_t size;

	FileInfo() : size() {}

	FileInfo(const FileInfo &other) : size(other.size) {} /* copy constructor*/
	FileInfo(FileInfo &&other) : size(other.size) {} /* move constructor*/
	FileInfo &operator=(const FileInfo &other) {
		size = other.size;
		return *this;
	}
} FileInfo;


/**
 * Prefetch Data structures
 */

typedef std::vector<std::pair<std::string, void *>> VectorPair;

template<typename I,
		typename std::enable_if<std::is_base_of<Input, I>::value>::type * = nullptr>
struct Event {
	size_t processId;
	uint64_t sequenceId;
	EventType type;
	t_mili time_stamp;
	I input;
	bool is_end;

	Event() : processId(0), sequenceId(0), input(), type(), time_stamp(0), is_end(false) {}

	Event(const Event &other) : processId(other.processId),
	                            sequenceId(other.sequenceId),
	                            time_stamp(other.time_stamp),
	                            type(other.type),
	                            input(other.input),
	                            is_end(other.is_end) {} /* copy constructor*/
	Event(Event &&other) : processId(other.processId),
	                       sequenceId(other.sequenceId),
	                       type(other.type),
	                       input(other.input),
	                       time_stamp(other.time_stamp),
	                       is_end(other.is_end) {} /* move constructor*/
	Event &operator=(const Event &other) {
		processId = other.processId;
		sequenceId = other.sequenceId;
		type = other.type;
		input = other.input;
		time_stamp = other.time_stamp;
		is_end = other.is_end;
		return *this;
	}
};

struct HDF5Event : public Event<HDF5Input> {

	HDF5Event() : Event<HDF5Input>() {} /* default constructor */
	HDF5Event(const HDF5Event &other) : Event<HDF5Input>(other) {} /* copy constructor */

	HDF5Event(HDF5Event &&other) : Event<HDF5Input>(other) {} /* move constructor*/
	HDF5Event &operator=(const HDF5Event &other) {
		Event<HDF5Input>::operator=(other);

		return *this;
	}
};

template<typename I,
		typename std::enable_if<std::is_base_of<Input, I>::value>::type * = nullptr>
struct GraphNode {
	size_t processId;
	size_t sequenceId;
	EventType type;
	I input;
	std::vector<size_t> links;
	t_mili start_time, end_time;

	GraphNode() : start_time(0), end_time(0) {}

	GraphNode(size_t processId, size_t sequenceId) : processId(processId), start_time(0), sequenceId(sequenceId),
	                                                 links() {}
};

typedef struct HDF5GraphNode : public GraphNode<HDF5Input> {
	HDF5GraphNode() {}

	HDF5GraphNode(size_t processId, size_t sequenceId) : GraphNode(processId, sequenceId) {}
} HDF5GraphNode;

struct RootGraphNode {
	size_t num_nodes;
	std::vector<size_t> links;

	RootGraphNode() : num_nodes(0), links() {}
};

/**
 * This is a comparator class for ordered map with key as Matrix.
 */
class MapCompare {
public:
	bool operator()(const Matrix &x, const Matrix &y) const {
		if (x.num_dimension != y.num_dimension)
			return true;
		if (x > y)
			return false;
		if (x.Contains(y))
			return false;
		return true;
	}
};
/**
 * This is for calculating hash of Matrix for unordered maps indexing.
 */

namespace std {
	template<>
	struct hash<Data> {
		size_t operator()(const Data &k) const {
			return 0;
		}
	};

	template<>
	struct hash<Matrix> {
		size_t operator()(const Matrix &k) const {
			size_t hash_val = hash<hsize_t>()(k.num_dimension);
			for (int r = 0; r < k.num_dimension; r++) {
				hash_val ^= hash<hsize_t>()(k.start_[r]);
				hash_val ^= hash<hsize_t>()(k.end_[r]);
			}

			return hash_val;
		}
	};

	template<>
	struct hash<pair<uint64_t, vector<pair<Matrix, pair<t_mili, t_mili>>>>> {
		size_t operator()(const pair<uint64_t, vector<pair<Matrix, pair<t_mili, t_mili>>>> &k) const {

			size_t hash_val = hash<uint64_t>()(k.first);
			for (int r = 0; r < k.second.size(); r++) {
				hash_val ^= hash<Matrix>()(k.second[r].first);
				hash_val ^= hash<t_mili>()(k.second[r].second.first);
				hash_val ^= hash<t_mili>()(k.second[r].second.second);
			}
			return hash_val;
		}
	};
}

struct PairCompare {
	bool operator()(const pair<t_mili, HDF5Event> &lhs, const pair<t_mili, HDF5Event> &rhs) const {
		return lhs.first > rhs.first;
	}
};


static std::size_t hash_value(Data const &b) {
	return 0;
}

static std::size_t hash_value(Matrix const &k) {
	size_t hash_val = hash<hsize_t>()(k.num_dimension);
	for (int r = 0; r < k.num_dimension; r++) {
		hash_val ^= hash<hsize_t>()(k.start_[r]);
		hash_val ^= hash<hsize_t>()(k.end_[r]);
	}

	return hash_val;
}

static std::size_t hash_value(CharStruct const &k) {
	std::string val(k.c_str());
	return hash<std::string>()(val);
}

/**
 * Operators
 */
std::ostream &operator<<(std::ostream &os, char const *m);

std::ostream &operator<<(std::ostream &os, uint8_t const &m);

std::ostream &operator<<(std::ostream &os, Layer const &m);

std::ostream &operator<<(std::ostream &os, Input const &m);

std::ostream &operator<<(std::ostream &os, ReadInput const &m);

std::ostream &operator<<(std::ostream &os, WriteInput const &m);

std::ostream &operator<<(std::ostream &os, std::vector<hsize_t> const &m);

std::ostream &operator<<(std::ostream &os, HDF5Input const &m);

std::ostream &operator<<(std::ostream &os, HDF5ReadInput const &m);

std::ostream &operator<<(std::ostream &os, HDF5WriteInput const &m);

std::ostream &operator<<(std::ostream &os, Output const &m);

std::ostream &operator<<(std::ostream &os, WriteOutput const &m);

std::ostream &operator<<(std::ostream &os, ReadOutput const &m);

std::ostream &operator<<(std::ostream &os, HDF5Output const &m);

std::ostream &operator<<(std::ostream &os, HDF5WriteOutput const &m);

std::ostream &operator<<(std::ostream &os, HDF5ReadOutput const &m);

template<typename I, typename O,
		typename std::enable_if<std::is_base_of<Input, I>::value>::type * = nullptr,
		typename std::enable_if<std::is_base_of<Output, O>::value>::type * = nullptr>
std::ostream &operator<<(std::ostream &os, Placement<I, O> const &m);

std::ostream &operator<<(std::ostream &os, Data const &m);

std::ostream &operator<<(std::ostream &os, Matrix const &m);



/**
 * Serialization by msgpack
 */
// User defined class template specialization
namespace clmdep_msgpack {
	MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
		namespace adaptor {
			namespace mv1=clmdep_msgpack::v1;

			/**
			 * add the compression lib variable to the serailization methods.
			 */
			template<>
			struct convert<HDF5Input> {
				mv1::object const &operator()(mv1::object const &o, HDF5Input &input) const {
					if (o.via.array.size != 12)
						throw mv1::type_error();
					input.filename = o.via.array.ptr[0].as<bip::string>();
					input.dataset_name = o.via.array.ptr[1].as<bip::string>();
					input.rank_ = o.via.array.ptr[2].as<int>();
					input.type_ = o.via.array.ptr[3].as<int64_t>();
					input.dimentions_ = o.via.array.ptr[4].as<std::array<hsize_t, MAX_DIMS>>();
					input.max_dimentions_ = o.via.array.ptr[5].as<std::array<hsize_t, MAX_DIMS>>();
					input.file_start_ = o.via.array.ptr[6].as<std::array<hsize_t, MAX_DIMS>>();
					input.file_end_ = o.via.array.ptr[7].as<std::array<hsize_t, MAX_DIMS>>();
					input.memory_start_ = o.via.array.ptr[8].as<std::array<hsize_t, MAX_DIMS>>();
					input.memory_dim_ = o.via.array.ptr[9].as<std::array<hsize_t, MAX_DIMS>>();
					input.operation_type = o.via.array.ptr[10].as<OperationType>();
					input.layer = o.via.array.ptr[11].as<Layer>();
					return o;
				}
			};

			template<>
			struct pack<HDF5Input> {
				template<typename Stream>
				packer<Stream> &operator()(mv1::packer<Stream> &o, HDF5Input const &input) const {
					o.pack_array(12);
					o.pack(input.filename);
					o.pack(input.dataset_name);
					o.pack(input.rank_);
					o.pack(input.type_);
					o.pack(input.dimentions_);
					o.pack(input.max_dimentions_);
					o.pack(input.file_start_);
					o.pack(input.file_end_);
					o.pack(input.memory_start_);
					o.pack(input.memory_dim_);
					o.pack(input.operation_type);
					o.pack(input.layer);
					return o;
				}
			};

			template<>
			struct object_with_zone<HDF5Input> {
				void operator()(mv1::object::with_zone &o, HDF5Input const &input) const {
					o.type = type::ARRAY;
					o.via.array.size = 12;
					o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
							sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
					o.via.array.ptr[0] = mv1::object(input.filename, o.zone);
					o.via.array.ptr[1] = mv1::object(input.dataset_name, o.zone);
					o.via.array.ptr[2] = mv1::object(input.rank_, o.zone);
					o.via.array.ptr[3] = mv1::object(input.type_, o.zone);
					o.via.array.ptr[4] = mv1::object(input.dimentions_, o.zone);
					o.via.array.ptr[5] = mv1::object(input.max_dimentions_, o.zone);
					o.via.array.ptr[6] = mv1::object(input.file_start_, o.zone);
					o.via.array.ptr[7] = mv1::object(input.file_end_, o.zone);
					o.via.array.ptr[8] = mv1::object(input.memory_start_, o.zone);
					o.via.array.ptr[9] = mv1::object(input.memory_dim_, o.zone);
					o.via.array.ptr[10] = mv1::object(input.operation_type, o.zone);
					o.via.array.ptr[11] = mv1::object(input.layer, o.zone);
				}
			};


			template<>
			struct convert<Matrix> {
				mv1::object const &operator()(mv1::object const &o, Matrix &input) const {
					input.num_dimension = o.via.array.ptr[0].as<hsize_t>();
					input.start_ = o.via.array.ptr[1].as<std::array<hsize_t, MAX_DIMS>>();
					input.original_start_ = o.via.array.ptr[2].as<std::array<hsize_t, MAX_DIMS>>();
					input.end_ = o.via.array.ptr[3].as<std::array<hsize_t, MAX_DIMS>>();
					return o;
				}
			};

			template<>
			struct pack<Matrix> {
				template<typename Stream>
				packer<Stream> &operator()(mv1::packer<Stream> &o, Matrix const &input) const {
					// packing member variables as an array.
					o.pack_array(4);
					o.pack(input.num_dimension);
					o.pack(input.start_);
					o.pack(input.original_start_);
					o.pack(input.end_);
					return o;
				}
			};

			template<>
			struct object_with_zone<Matrix> {
				void operator()(mv1::object::with_zone &o, Matrix const &input) const {
					o.type = type::ARRAY;
					o.via.array.size = 4;
					o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
							sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
					o.via.array.ptr[0] = mv1::object(input.num_dimension, o.zone);
					o.via.array.ptr[1] = mv1::object(input.start_, o.zone);
					o.via.array.ptr[2] = mv1::object(input.original_start_, o.zone);
					o.via.array.ptr[3] = mv1::object(input.end_, o.zone);
				}
			};

			template<>
			struct convert<Layer> {
				mv1::object const &operator()(mv1::object const &o, Layer &input) const {
					input.id_ = o.via.array.ptr[0].as<uint8_t>();
					input.capacity_mb_ = o.via.array.ptr[1].as<float>();
					input.layer_loc = o.via.array.ptr[2].as<CharStruct>();
					input.bandwidth_mbps_ = o.via.array.ptr[3].as<float>();
					input.direct_io = o.via.array.ptr[4].as<bool>();
					input.io_client_type = o.via.array.ptr[5].as<IOClientType>();
					return o;
				}
			};

			template<>
			struct pack<Layer> {
				template<typename Stream>
				packer<Stream> &operator()(mv1::packer<Stream> &o, Layer const &input) const {
					// packing member variables as an array.
					o.pack_array(6);
					o.pack(input.id_);
					o.pack(input.capacity_mb_);
					o.pack(input.layer_loc);
					o.pack(input.bandwidth_mbps_);
					o.pack(input.direct_io);
					o.pack(input.io_client_type);
					return o;
				}
			};

			template<>
			struct object_with_zone<Layer> {
				void operator()(mv1::object::with_zone &o, Layer const &input) const {
					o.type = type::ARRAY;
					o.via.array.size = 6;
					o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
							sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
					o.via.array.ptr[0] = mv1::object(input.id_, o.zone);
					o.via.array.ptr[1] = mv1::object(input.capacity_mb_, o.zone);
					o.via.array.ptr[2] = mv1::object(input.layer_loc, o.zone);
					o.via.array.ptr[3] = mv1::object(input.bandwidth_mbps_, o.zone);
					o.via.array.ptr[4] = mv1::object(input.direct_io, o.zone);
					o.via.array.ptr[5] = mv1::object(input.io_client_type, o.zone);
				}
			};

			template<>
			struct convert<HDF5Event> {
				mv1::object const &operator()(mv1::object const &o, HDF5Event &input) const {
					input.processId = o.via.array.ptr[0].as<size_t>();
					input.sequenceId = o.via.array.ptr[1].as<uint64_t>();
					input.type = o.via.array.ptr[2].as<EventType>();
					input.input = o.via.array.ptr[3].as<HDF5Input>();
					input.time_stamp = o.via.array.ptr[4].as<t_mili>();
					input.is_end = o.via.array.ptr[5].as<bool>();
					return o;
				}
			};

			template<>
			struct pack<HDF5Event> {
				template<typename Stream>
				packer<Stream> &operator()(mv1::packer<Stream> &o, HDF5Event const &input) const {
					// packing member variables as an array.
					o.pack_array(6);
					o.pack(input.processId);
					o.pack(input.sequenceId);
					o.pack(input.type);
					o.pack(input.input);
					o.pack(input.time_stamp);
					o.pack(input.is_end);
					return o;
				}
			};

			template<>
			struct object_with_zone<HDF5Event> {
				void operator()(mv1::object::with_zone &o, HDF5Event const &input) const {
					o.type = type::ARRAY;
					o.via.array.size = 6;
					o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
							sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
					o.via.array.ptr[0] = mv1::object(input.processId, o.zone);
					o.via.array.ptr[1] = mv1::object(input.sequenceId, o.zone);
					o.via.array.ptr[2] = mv1::object(input.type, o.zone);
					o.via.array.ptr[3] = mv1::object(input.input, o.zone);
					o.via.array.ptr[4] = mv1::object(input.time_stamp, o.zone);
					o.via.array.ptr[5] = mv1::object(input.is_end, o.zone);
				}
			};

			template<>
			struct convert<FileInfo> {
				mv1::object const &operator()(mv1::object const &o, FileInfo &input) const {
					input.size = o.via.array.ptr[0].as<uint32_t>();
					return o;
				}
			};

			template<>
			struct pack<FileInfo> {
				template<typename Stream>
				packer<Stream> &operator()(mv1::packer<Stream> &o, FileInfo const &input) const {
					// packing member variables as an array.
					o.pack_array(1);
					o.pack(input.size);
					return o;
				}
			};

			template<>
			struct object_with_zone<FileInfo> {
				void operator()(mv1::object::with_zone &o, FileInfo const &input) const {
					o.type = type::ARRAY;
					o.via.array.size = 1;
					o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
							sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
					o.via.array.ptr[0] = mv1::object(input.size, o.zone);
				}
			};

			template<>
			struct convert<HDF5GraphNode> {
				mv1::object const &operator()(mv1::object const &o, HDF5GraphNode &input) const {
					input.processId = o.via.array.ptr[0].as<size_t>();
					input.sequenceId = o.via.array.ptr[1].as<size_t>();
					input.input = o.via.array.ptr[2].as<HDF5Input>();
					input.links = o.via.array.ptr[3].as<std::vector<size_t>>();
					input.start_time = o.via.array.ptr[4].as<t_mili>();
					input.type = o.via.array.ptr[5].as<EventType>();
					input.end_time = o.via.array.ptr[6].as<t_mili>();
					return o;
				}
			};

			template<>
			struct pack<HDF5GraphNode> {
				template<typename Stream>
				packer<Stream> &operator()(mv1::packer<Stream> &o, HDF5GraphNode const &input) const {
					// packing member variables as an array.
					o.pack_array(7);
					o.pack(input.processId);
					o.pack(input.sequenceId);
					o.pack(input.input);
					o.pack(input.links);
					o.pack(input.start_time);
					o.pack(input.type);
					o.pack(input.end_time);
					return o;
				}
			};

			template<>
			struct object_with_zone<HDF5GraphNode> {
				void operator()(mv1::object::with_zone &o, HDF5GraphNode const &input) const {
					o.type = type::ARRAY;
					o.via.array.size = 7;
					o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
							sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
					o.via.array.ptr[0] = mv1::object(input.processId, o.zone);
					o.via.array.ptr[1] = mv1::object(input.sequenceId, o.zone);
					o.via.array.ptr[2] = mv1::object(input.input, o.zone);
					o.via.array.ptr[3] = mv1::object(input.links, o.zone);
					o.via.array.ptr[4] = mv1::object(input.start_time, o.zone);
					o.via.array.ptr[5] = mv1::object(input.type, o.zone);
					o.via.array.ptr[6] = mv1::object(input.end_time, o.zone);
				}
			};


			template<>
			struct convert<RootGraphNode> {
				mv1::object const &operator()(mv1::object const &o, RootGraphNode &input) const {
					input.num_nodes = o.via.array.ptr[0].as<size_t>();
					input.links = o.via.array.ptr[1].as<std::vector<size_t>>();
					return o;
				}
			};

			template<>
			struct pack<RootGraphNode> {
				template<typename Stream>
				packer<Stream> &operator()(mv1::packer<Stream> &o, RootGraphNode const &input) const {
					// packing member variables as an array.
					o.pack_array(2);
					o.pack(input.num_nodes);
					o.pack(input.links);
					return o;
				}
			};

			template<>
			struct object_with_zone<RootGraphNode> {
				void operator()(mv1::object::with_zone &o, RootGraphNode const &input) const {
					o.type = type::ARRAY;
					o.via.array.size = 2;
					o.via.array.ptr = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
							sizeof(mv1::object) * o.via.array.size, MSGPACK_ZONE_ALIGNOF(mv1::object)));
					o.via.array.ptr[0] = mv1::object(input.num_nodes, o.zone);
					o.via.array.ptr[1] = mv1::object(input.links, o.zone);
				}
			};


			template<typename T, typename Alloc>
			struct as<bip::vector<T, Alloc>, typename std::enable_if<clmdep_msgpack::has_as<T>::value>::type> {
				bip::vector<T, Alloc> operator()(const clmdep_msgpack::object &o) const {
					if (o.type != clmdep_msgpack::type::ARRAY) { throw clmdep_msgpack::type_error(); }
					bip::vector<T, Alloc> v;
					v.reserve(o.via.array.size);
					if (o.via.array.size > 0) {
						clmdep_msgpack::object *p = o.via.array.ptr;
						clmdep_msgpack::object *const pend = o.via.array.ptr + o.via.array.size;
						do {
							v.push_back(p->as<T>());
							++p;
						} while (p < pend);
					}
					return v;
				}
			};

			template<typename T, typename Alloc>
			struct convert<bip::vector<T, Alloc> > {
				clmdep_msgpack::object const &
				operator()(clmdep_msgpack::object const &o, bip::vector<T, Alloc> &v) const {
					if (o.type != clmdep_msgpack::type::ARRAY) { throw clmdep_msgpack::type_error(); }
					v.resize(o.via.array.size);
					if (o.via.array.size > 0) {
						clmdep_msgpack::object *p = o.via.array.ptr;
						clmdep_msgpack::object *const pend = o.via.array.ptr + o.via.array.size;
						typename bip::vector<T, Alloc>::iterator it = v.begin();
						do {
							p->convert(*it);
							++p;
							++it;
						} while (p < pend);
					}
					return o;
				}
			};

			template<typename T, typename Alloc>
			struct pack<bip::vector<T, Alloc> > {
				template<typename Stream>
				clmdep_msgpack::packer<Stream> &
				operator()(clmdep_msgpack::packer<Stream> &o, const bip::vector<T, Alloc> &v) const {
					uint32_t size = checked_get_container_size(v.size());
					o.pack_array(size);
					for (typename bip::vector<T, Alloc>::const_iterator it(v.begin()), it_end(v.end());
					     it != it_end; ++it) {
						o.pack(*it);
					}
					return o;
				}
			};

			template<typename T, typename Alloc>
			struct object_with_zone<bip::vector<T, Alloc> > {
				void operator()(clmdep_msgpack::object::with_zone &o, const bip::vector<T, Alloc> &v) const {
					o.type = clmdep_msgpack::type::ARRAY;
					if (v.empty()) {
						o.via.array.ptr = MSGPACK_NULLPTR;
						o.via.array.size = 0;
					} else {
						uint32_t size = checked_get_container_size(v.size());
						clmdep_msgpack::object *p = static_cast<clmdep_msgpack::object *>(o.zone.allocate_align(
								sizeof(clmdep_msgpack::object) * size, MSGPACK_ZONE_ALIGNOF(clmdep_msgpack::object)));
						clmdep_msgpack::object *const pend = p + size;
						o.via.array.ptr = p;
						o.via.array.size = size;
						typename bip::vector<T, Alloc>::const_iterator it(v.begin());
						do {
#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif // (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
							*p = clmdep_msgpack::object(*it, o.zone);
#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif // (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)) && !defined(__clang__)
							++p;
							++it;
						} while (p < pend);
					}
				}
			};

			template<>
			struct convert<bip::string> {
				clmdep_msgpack::object const &operator()(clmdep_msgpack::object const &o, bip::string &v) const {
					switch (o.type) {
						case clmdep_msgpack::type::BIN:
							v.assign(o.via.bin.ptr, o.via.bin.size);
							break;
						case clmdep_msgpack::type::STR:
							v.assign(o.via.str.ptr, o.via.str.size);
							break;
						default:
							throw clmdep_msgpack::type_error();
							break;
					}
					return o;
				}
			};

			template<>
			struct pack<bip::string> {
				template<typename Stream>
				clmdep_msgpack::packer<Stream> &
				operator()(clmdep_msgpack::packer<Stream> &o, const bip::string &v) const {
					uint32_t size = checked_get_container_size(v.size());
					o.pack_str(size);
					o.pack_str_body(v.data(), size);
					return o;
				}
			};

			template<>
			struct object<bip::string> {
				void operator()(clmdep_msgpack::object &o, const bip::string &v) const {
					uint32_t size = checked_get_container_size(v.size());
					o.type = clmdep_msgpack::type::STR;
					o.via.str.ptr = v.data();
					o.via.str.size = size;
				}
			};

			template<>
			struct object_with_zone<bip::string> {
				void operator()(clmdep_msgpack::object::with_zone &o, const bip::string &v) const {
					uint32_t size = checked_get_container_size(v.size());
					o.type = clmdep_msgpack::type::STR;
					char *ptr = static_cast<char *>(o.zone.allocate_align(size, MSGPACK_ZONE_ALIGNOF(char)));
					o.via.str.ptr = ptr;
					o.via.str.size = size;
					std::memcpy(ptr, v.data(), v.size());
				}
			};
		}
	}
}




#endif //SCORE_DATA_STRUCTURES_H
