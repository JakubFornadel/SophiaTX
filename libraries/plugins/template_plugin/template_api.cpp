#include <sophiatx/plugins/template/template_plugin.hpp>
#include <sophiatx/plugins/template/template_api.hpp>
#include <sophiatx/plugins/template/template_objects.hpp>

namespace sophiatx { namespace plugins { namespace template_plugin {

namespace detail {

class template_api_impl
{
public:
   template_api_impl(template_plugin& plugin) : _app(plugin.app()), _db( plugin.app()->get_plugin< sophiatx::plugins::chain::chain_plugin >().db() ) {}

   DECLARE_API_IMPL((example_call))

   appbase::application* _app;
   std::shared_ptr<database_interface> _db;
};

DEFINE_API_IMPL(template_api_impl, example_call)
{
   example_call_return final_result;

   final_result.sum = args.arg1 + args.arg2;
   final_result.mul = args.arg1 * args.arg2;

   return final_result;
}

} // detail

template_api::template_api(template_plugin& plugin): my( new detail::template_api_impl(plugin) )
{
   JSON_RPC_REGISTER_API( SOPHIATX_TEMPLATE_PLUGIN_NAME, plugin.app() );
}

template_api::~template_api()
{
   JSON_RPC_DEREGISTER_API( SOPHIATX_TEMPLATE_PLUGIN_NAME, my->_app );
}

DEFINE_READ_APIS( template_api, (example_call) )

} } } // sophiatx::plugins::template_plugin
