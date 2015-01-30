/*
 * coltbitmap.h
 *
 *  Created on: Jul 28, 2014
 *      Author: paul
 */

#ifndef COLTBITMAP_H_
#define COLTBITMAP_H_

#define COLT_MAP_BYTES_IN_ELEM sizeof(long)
#define COLT_MAP_BITS_IN_ELEM (COLT_MAP_BYTES_IN_ELEM*4)
#define COLT_MAP_HIGH_BIT 0x80000000

typedef struct {
	int	id;
	char file_name[100];
	int min_value;
	int max_value;
	int num_elements;
} colt_bitmap_identifier;

class coltbitmap: public coltthru {
public:
	long 	*map;
	long	num_elements;
	long	initial_disp;
	long	min_value;
	long 	max_value;

	coltbitmap(colt_base &in, char *low, char *high, int all_bits = 0);
	coltbitmap(colt_base &in, char *destination_file_name=NULL, int all_bits = 0);
	coltbitmap(colt_base &b, char *file, char *low, char *high, int all_bits = 0);
	coltbitmap(char *fname);
	~coltbitmap();

	int is_set(long val) {
		if(val < min_value || val > max_value)
			return 0;
		int num = val - initial_disp;
		int elem = num / COLT_MAP_BITS_IN_ELEM;
		int disp = num % COLT_MAP_BITS_IN_ELEM;
		return map[elem] & (COLT_MAP_HIGH_BIT >> (disp));
	};
	void set(long val) {
		if(val < min_value || val > max_value)
			return;
		int num = val - initial_disp;
		int elem = num / COLT_MAP_BITS_IN_ELEM;
		unsigned long disp = num % COLT_MAP_BITS_IN_ELEM;
//		cout << "qqq " << val << ":" << elem << ":" << disp << ":" << (COLT_MAP_HIGH_BIT >> (disp)) << "\n";
		map[elem] = map[elem] | (COLT_MAP_HIGH_BIT >> (disp));
	};
	void allocate(int min, int max) {
		min_value = min;
		max_value = max;
		int minv = min / COLT_MAP_BITS_IN_ELEM;
		initial_disp = minv * COLT_MAP_BITS_IN_ELEM;
		int maxv = (max / COLT_MAP_BITS_IN_ELEM) + 1;
		num_elements = maxv-minv;
		map = (long *) malloc( num_elements * COLT_MAP_BYTES_IN_ELEM );
		for(int i=0; i<num_elements; i++)
			map[i] = 0;
	}
	void show() {
		cout << min_value << ":" << max_value << "\n";
		if(!num_elements)
			std::cout << "no elements\n";
		else
			for(int i=0; i<num_elements; i++) {
				char tmp[100];
				sprintf(tmp, "%8lx", (unsigned long) map[i]);
				std::cout << initial_disp + i*COLT_MAP_BITS_IN_ELEM
						<< "\t" << tmp << "\n";
//				bitset<COLT_MAP_BITS_IN_ELEM> x(map[i]);
//				std::cout << initial_disp + i*COLT_MAP_BITS_IN_ELEM
//						<< "\t" << x << "\n";
			}
		for(int j=min_value; j<=max_value; j++)
			if(is_set(j))
				cout << j << " ";
		cout << "\n";
	};
	int lowest_bit_set(int start=-1);
	int highest_bit_set(int start=-1);
	void xlate_to_range_list();
	int all_bits_set() { return map == NULL; };

	virtual	int serialize_id_num();
	virtual int write_config(FILE *);
	virtual int *read_config(int *);
	int show_status(char *baseptr, int indent=0);
	int to_string(char *x);
	char *from_string(char *input);
	int contains(int rec_num);

	void set_begin_end_index(int beg, int end=-1);
	char **fields(int rec_num);
	void process_all();
	int preprocess();
	int process(int rec_num);
	void postprocess();
};

#endif /* COLTBITMAP_H_ */
