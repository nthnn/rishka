mod process;
mod env;
mod io;

fn main() {
    process::check_req_deps();
    env::check_req_env();
}
