
#include "libed2k/session.hpp"
#include "libed2k/session_impl.hpp"
#include "libed2k/peer_connection.hpp"
#include "libed2k/server_connection.hpp"
#include "libed2k/transfer_handle.hpp"

namespace libed2k {

    void add_transfer_params::dump() const
    {
        DBG("add_transfer_params::dump");
        DBG("file hash: " << file_hash << " all hashes size: " << piece_hash.all_hashes().size());
        DBG("file path: " << convert_to_native(file_path.string()));
        DBG("file size: " << file_size);
        DBG("accepted: " << m_accepted
                            << " requested: " << m_requested
                            << " transf: " << m_transferred
                            << " priority: " << m_priority);
    }

void session::init(const fingerprint& id, const char* listen_interface,
                   const session_settings& settings)
{
    m_impl.reset(new aux::session_impl(id, listen_interface, settings));
}

session::~session()
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);

    // if there is at least one destruction-proxy
    // abort the session and let the destructor
    // of the proxy to syncronize
    if (!m_impl.unique()) m_impl->abort();
}

transfer_handle session::add_transfer(const add_transfer_params& params)
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);

    error_code ec;
    transfer_handle ret = m_impl->add_transfer(params, ec);
    if (ec) throw libed2k_exception(ec);
    return ret;
}

std::vector<transfer_handle> session::add_transfer_dir(const fs::path& dir)
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);

    error_code ec;
    std::vector<transfer_handle> ret = m_impl->add_transfer_dir(dir, ec);
    if (ec) throw libed2k_exception(ec);
    return ret;
}

std::auto_ptr<alert> session::pop_alert()
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    return m_impl->pop_alert();
}

void session::set_alert_dispatch(boost::function<void(alert const&)> const& fun)
{
    // this function deliberately doesn't acquire the mutex
    return m_impl->set_alert_dispatch(fun);
}

alert const* session::wait_for_alert(time_duration max_wait)
{
    // this function deliberately doesn't acquire the mutex
    return m_impl->wait_for_alert(max_wait);
}

void session::set_alert_mask(boost::uint32_t m)
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    m_impl->set_alert_mask(m);
}

size_t session::set_alert_queue_size_limit(size_t queue_size_limit_)
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    return m_impl->set_alert_queue_size_limit(queue_size_limit_);
}

void session::post_search_request(search_request& ro)
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    m_impl->post_search_request(ro);
}

void session::post_search_more_result_request()
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    m_impl->post_search_more_result_request();
}

void session::post_sources_request(const md4_hash& hFile, boost::uint64_t nSize)
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    m_impl->post_sources_request(hFile, nSize);
}

void session::post_message(client_id_type nIP, int nPort, const std::string& strMessage)
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    m_impl->post_message(nIP, nPort, strMessage);
}

transfer_handle session::find_transfer(const md4_hash & hash) const
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    return m_impl->find_transfer_handle(hash);
}

std::vector<transfer_handle> session::get_transfers() const
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    return m_impl->get_transfers();
}

int session::download_rate_limit() const
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    return m_impl->m_settings.download_rate_limit;
}

int session::upload_rate_limit() const
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    return m_impl->m_settings.upload_rate_limit;
}

void session::server_conn_start()
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    m_impl->server_conn_start();
}

void session::server_conn_stop()
{
    boost::mutex::scoped_lock l(m_impl->m_mutex);
    m_impl->server_conn_stop();
}



}
