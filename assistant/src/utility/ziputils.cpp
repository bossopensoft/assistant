#include "ziputils.h"
#include <stdio.h>
#include <QDir>
#include "mz_zip.h"
#include "mz.h"
#include "mz_os.h"
#include "mz_strm.h"
#include "mz_strm_zstd.h"
#include "mz_strm_buf.h"
#include "mz_strm_split.h"
#include "mz_zip_rw.h"
#include "mz_crypt.h"


typedef struct minizip_opt_s {
    uint8_t     include_path;
    int16_t     compress_level;
    uint8_t     compress_method;
    uint8_t     overwrite;
    uint8_t     append;
    int64_t     disk_size;
    uint8_t     follow_links;
    uint8_t     store_links;
    uint8_t     zip_cd;
    int32_t     encoding;
    uint8_t     verbose;
    uint8_t     aes;
    const char *cert_path;
    const char *cert_pwd;
    void* userdata;
} minizip_opt;


int32_t minizip_extract_entry_cb(void *handle, void *userdata, mz_zip_file *file_info, const char *path) {
    MZ_UNUSED(handle);
    MZ_UNUSED(userdata);
    MZ_UNUSED(path);

    /* Print the current entry extracting */
    printf("Extracting %s\n", file_info->filename);
    return MZ_OK;
}

int32_t minizip_extract_progress_cb(void *handle, void *userdata, mz_zip_file *file_info, int64_t position) {
    minizip_opt *options = (minizip_opt *)userdata;
    double progress = 0;
    uint8_t raw = 0;

    MZ_UNUSED(userdata);

    mz_zip_reader_get_raw(handle, &raw);

    if (raw && file_info->compressed_size > 0)
        progress = ((double)position / file_info->compressed_size) * 100;
    else if (!raw && file_info->uncompressed_size > 0)
        progress = ((double)position / file_info->uncompressed_size) * 100;

    /* Print the progress of the current extraction */
    if (options->verbose)
        printf("%s - %" PRId64 " / %" PRId64 " (%.02f%%)\n", file_info->filename, position,
            file_info->uncompressed_size, progress);

    return MZ_OK;
}

int32_t minizip_extract_overwrite_cb(void *handle, void *userdata, mz_zip_file *file_info, const char *path) {
    minizip_opt *options = (minizip_opt *)userdata;

    MZ_UNUSED(handle);
    MZ_UNUSED(file_info);

    /* Verify if we want to overwrite current entry on disk */
    if (options->overwrite == 0) {
        char rep = 0;
        do {
            char answer[128];
            printf("The file %s exists. Overwrite ? [y]es, [n]o, [A]ll: ", path);
            if (scanf("%1s", answer) != 1)
                exit(EXIT_FAILURE);
            rep = answer[0];
            if ((rep >= 'a') && (rep <= 'z'))
                rep -= 0x20;
        } while ((rep != 'Y') && (rep != 'N') && (rep != 'A'));

        if (rep == 'N')
            return MZ_EXIST_ERROR;
        if (rep == 'A')
            options->overwrite = 1;
    }

    return MZ_OK;
}

int extract(const QString &file, const QString &outPath)
{
    QDir dir(outPath);
    if (!dir.exists()){
        dir.mkpath(outPath);
    }
    void *reader = NULL;
    int32_t err = MZ_OK;
    int32_t err_close = MZ_OK;

    minizip_opt _options;
    memset(&_options, 0, sizeof(_options));
    _options.compress_method = MZ_COMPRESS_METHOD_DEFLATE;
    _options.compress_level = MZ_COMPRESS_LEVEL_DEFAULT;
    _options.overwrite = 1;
    _options.include_path = 0;

    mz_zip_reader_create(&reader);
    mz_zip_reader_set_pattern(reader, nullptr, 1);
//    mz_zip_reader_set_password(reader, _pwd.c_str());
    mz_zip_reader_set_encoding(reader, _options.encoding);
    mz_zip_reader_set_entry_cb(reader, &_options, minizip_extract_entry_cb);
    mz_zip_reader_set_progress_cb(reader, &_options, minizip_extract_progress_cb);
    mz_zip_reader_set_overwrite_cb(reader, &_options, minizip_extract_overwrite_cb);

    err = mz_zip_reader_open_file(reader, file.toLocal8Bit().data());

     if (err == MZ_OK){
        err = mz_zip_reader_save_all(reader, outPath.toLocal8Bit().data());
     }

     err_close = mz_zip_reader_close(reader);
     if (err_close != MZ_OK) {
         err = err_close;
     }

     mz_zip_reader_delete(&reader);

     return err;
    return 0;
}
