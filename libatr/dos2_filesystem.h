#pragma once

#include "filesystem.h"

class dos2 : public filesystem
{
public:

	dos2(disk * d);
	~dos2();

	static filesystem * format(disk * d);
	static bool detect(disk * d);

	std::string name() override;
	const property * properties() override;


	disk::sector_num free_sector_count() override;

	disk::sector_num get_dos_first_sector() override;
	void set_dos_first_sector(disk::sector_num sector) override;

	filesystem::dir * root_dir() override;

	class dos2_file : public filesystem::file
	{
	public:

		dos2_file(dos2 & fs, disk::sector_num dir_sector, size_t dir_pos, int file_no, disk::sector_num first_sec, disk::sector_num sec_cnt, bool writing);
		~dos2_file();

		bool eof() override;
		byte read() override;
		void write(byte b) override;
		disk::sector_num first_sector() override;

	protected:

		void write_sec(disk::sector_num next);
		bool sector_end();
		disk::sector_num sector_next();
		bool next_sector();

		dos2 & fs;

		disk::sector_num first_sec;

		// current position
		disk::sector_num sector;
		size_t pos;

		//byte * buf;

		bool writing;

		// dir
		disk::sector_num dir_sector;
		size_t           dir_pos;		// position in sector
		int				 file_no;
		
		size_t sec_cnt;					// size if not known yet
		size_t size;					// size in bytes
	};

	class dos2_dir : public filesystem::dir
	{
	public:
		dos2_dir(dos2 & fs, disk::sector_num sector);
		void next() override;
		bool at_end() override;
		std::string name() override;
		size_t sec_size() override;
		file * open_file() override;
		size_t size() override;
		bool is_deleted() override;
		file * create_file(char * name) override;
		void format();

	protected:
		int alloc_entry(char * name, byte flags, disk::sector_num first_sec, disk::sector_num * p_sector, size_t * p_offset);
		dos2 & fs;
		disk::sector_num first_sector;
		disk::sector_num end_sector;
		disk::sector_num sector;
		size_t           pos;		// position in sector
		int				 file_no;

		//byte * buf;
	};

protected:

	// DIR
	void dir_format();
	//byte * dir_buf;

	// VTOC
	
	inline void switch_sector_used(disk::sector_num sec) {
		vtoc_buf[10 + sec / 8] ^= (128 >> (sec & 7));
	}
	
	virtual disk::sector_num alloc_sector();
	virtual void free_sector(disk::sector_num sector);

	virtual void vtoc_format();
	virtual void vtoc_read();
	virtual void vtoc_write();

	byte * vtoc_buf;
	bool vtoc_dirty;
};
