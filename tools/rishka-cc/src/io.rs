use std::fs;

pub fn directory_exists(directory_path: &str) -> bool {
    match fs::metadata(directory_path) {
        Ok(meta)=> meta.is_dir(),
        Err(_)=> false
    }
}