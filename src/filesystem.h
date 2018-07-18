#pragma once
#include "disk.h"
#include <string>

class filesystem
{
public:
	filesystem(disk * d) : d(d) {}
	virtual ~filesystem() {};
	virtual std::string name() = 0;

	const char * dos_lo = "DOSSEC_LO";
	const char * dos_hi = "DOSSEC_HI";

	struct property
	{
		const char * name;
		disk::sector_num sector;
		size_t offset;
		size_t size;
	};

	virtual const property * properties() = 0;

	const property * find_property(const std::string & name);
	void set_property(const property * prop, const std::string & value);
	void set_property(const property * prop, int value);
	byte get_property_byte(const char * name);

	void set_dos(disk::sector_num sector);
	disk::sector_num get_dos();

	class file
	{
	public:
		virtual bool eof() = 0;
		virtual byte read() = 0;
		virtual void write(byte b) = 0;
		virtual void write(byte * data, size_t size);
		void save(const std::string & filename);
		void import(const std::string & filename);
		virtual ~file() {};
		virtual disk::sector_num first_sector() = 0;
	};

	class dir
	{
	public:
		virtual std::string name() = 0;
		virtual size_t sec_size() = 0;
		virtual void next() = 0;
		virtual bool at_end() = 0;
		virtual file * open_file() = 0;
	};

	disk * get_disk();
	virtual file * create_file(char * name) = 0;
	virtual dir * root_dir() = 0;

	size_t sector_size() {
		return d->sector_size();
	}

	disk::sector_num sector_count() {
		return d->sector_count();
	}

protected:

	void write_sector(disk::sector_num num, byte * data)
	{
		d->write_sector(num, data);
	}

	void read_sector(disk::sector_num num, byte * data)
	{
		d->read_sector(num, data);
	}

	disk * d;
};
